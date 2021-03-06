// Copyright 2010-2014 RethinkDB, all rights reserved.
#include "clustering/administration/servers/server_config.hpp"

#include "clustering/administration/datum_adapter.hpp"
#include "clustering/administration/metadata.hpp"
#include "clustering/administration/servers/config_client.hpp"
#include "concurrency/cross_thread_signal.hpp"

bool convert_server_config_from_datum(
        ql::datum_t datum,
        server_id_t *server_id_out,
        server_config_t *server_config_out,
        admin_err_t *error_out) {
    converter_from_datum_object_t converter;
    if (!converter.init(datum, error_out)) {
        return false;
    }

    ql::datum_t name_datum;
    if (!converter.get("name", &name_datum, error_out)) {
        return false;
    }
    if (!convert_name_from_datum(name_datum, "server name",
            &server_config_out->name, error_out)) {
        error_out->msg = "In `name`: " + error_out->msg;
        return false;
    }

    ql::datum_t id_datum;
    if (!converter.get("id", &id_datum, error_out)) {
        return false;
    }
    if (!convert_server_id_from_datum(id_datum, server_id_out, error_out)) {
        error_out->msg = "In `id`: " + error_out->msg;
        return false;
    }

    ql::datum_t tags_datum;
    if (!converter.get("tags", &tags_datum, error_out)) {
        return false;
    }
    if (!convert_set_from_datum<name_string_t>(
            [](ql::datum_t datum2, name_string_t *val2_out, admin_err_t *error2_out) {
                return convert_name_from_datum(
                    datum2, "server tag", val2_out, error2_out);
            },
            true, /* don't complain if a tag appears twice */
            tags_datum, &server_config_out->tags, error_out)) {
        error_out->msg = "In `tags`: " + error_out->msg;
        return false;
    }

    ql::datum_t cache_size_datum;
    if (!converter.get("cache_size_mb", &cache_size_datum, error_out)) {
        return false;
    }
    if (cache_size_datum == ql::datum_t("auto")) {
        server_config_out->cache_size_bytes = optional<uint64_t>();
    } else if (cache_size_datum.get_type() == ql::datum_t::R_NUM) {
        double cache_size_mb = cache_size_datum.as_num();
        if (cache_size_mb * MEGABYTE >
                static_cast<double>(std::numeric_limits<int64_t>::max())) {
            *error_out = admin_err_t{
                "In `cache_size_mb`: Value is too big.",
                query_state_t::FAILED};
            return false;
        }
        if (cache_size_mb < 0) {
            *error_out = admin_err_t{
                "In `cache_size_mb`: Cache size cannot be negative.",
                query_state_t::FAILED};
            return false;
        }
        server_config_out->cache_size_bytes =
            optional<uint64_t>(cache_size_mb * MEGABYTE);
    } else {
        *error_out = admin_err_t{
            "In `cache_size_mb`: Expected a number or 'auto', got "
            + cache_size_datum.print(),
            query_state_t::FAILED};
        return false;
    }

    if (!converter.check_no_extra_keys(error_out)) {
        return false;
    }

    return true;
}

server_config_artificial_table_backend_t::server_config_artificial_table_backend_t(
        rdb_context_t *rdb_context,
        lifetime_t<name_resolver_t const &> name_resolver,
        watchable_map_t<peer_id_t, cluster_directory_metadata_t> *_directory,
        server_config_client_t *_server_config_client)
    : common_server_artificial_table_backend_t(
        name_string_t::guarantee_valid("server_config"),
        rdb_context,
        name_resolver,
        _server_config_client,
        _directory) {
}

server_config_artificial_table_backend_t::~server_config_artificial_table_backend_t() {
    begin_changefeed_destruction();
}

bool server_config_artificial_table_backend_t::format_row(
        UNUSED auth::user_context_t const &user_context,
        server_id_t const & server_id,
        UNUSED peer_id_t const & peer_id,
        cluster_directory_metadata_t const & metadata,
        UNUSED signal_t *interruptor_on_home,
        ql::datum_t *row_out,
        UNUSED admin_err_t *error_out) {
    ql::datum_object_builder_t builder;

    builder.overwrite("name",
        convert_name_to_datum(metadata.server_config.config.name));
    builder.overwrite("id", convert_uuid_to_datum(server_id.get_uuid()));
    builder.overwrite("tags", convert_set_to_datum<name_string_t>(
            &convert_name_to_datum, metadata.server_config.config.tags));

    optional<uint64_t> cache_size_bytes =
        metadata.server_config.config.cache_size_bytes;
    if (static_cast<bool>(cache_size_bytes)) {
        builder.overwrite("cache_size_mb",
            ql::datum_t(static_cast<double>(*cache_size_bytes) / MEGABYTE));
    } else {
        builder.overwrite("cache_size_mb", ql::datum_t("auto"));
    }

    *row_out = std::move(builder).to_datum();

    return true;
}

bool server_config_artificial_table_backend_t::write_row(
        UNUSED auth::user_context_t const &user_context,
        ql::datum_t primary_key,
        UNUSED bool pkey_was_autogenerated,
        ql::datum_t *new_value_inout,
        signal_t *interruptor_on_caller,
        admin_err_t *error_out) {
    cross_thread_signal_t interruptor_on_home(interruptor_on_caller, home_thread());
    on_thread_t thread_switcher(home_thread());
    new_mutex_in_line_t write_mutex_in_line(&write_mutex);
    wait_interruptible(write_mutex_in_line.acq_signal(), &interruptor_on_home);
    server_id_t server_id;
    peer_id_t peer_id;
    cluster_directory_metadata_t metadata;
    if (!lookup(primary_key, &server_id, &peer_id, &metadata)) {
        if (new_value_inout->has()) {
            *error_out = admin_err_t{"It's illegal to insert new rows into the "
                                     "`rethinkdb.server_config` system table.",
                                     query_state_t::FAILED};
            return false;
        } else {
            /* The user is re-deleting an already-absent row. OK. */
            return true;
        }
    }
    if (!new_value_inout->has()) {
        *error_out = admin_err_t{
            "It's illegal to delete rows from the `rethinkdb.server_config` "
            "system table.",
            query_state_t::FAILED};
        return false;
    }
    server_id_t new_server_id;
    server_config_t new_server_config;
    if (!convert_server_config_from_datum(*new_value_inout, &new_server_id,
            &new_server_config, error_out)) {
        error_out->msg = "The row you're trying to put into `rethinkdb.server_config` "
            "has the wrong format. " + error_out->msg;
        return false;
    }
    guarantee(server_id == new_server_id, "artificial_table_t should ensure that "
        "primary key is unchanged.");
    if (!server_config_client->set_config(server_id, metadata.server_config.config.name,
            new_server_config, &interruptor_on_home, error_out)) {
        return false;
    }
    return true;
}

