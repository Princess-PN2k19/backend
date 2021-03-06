// Copyright 2010-2014 RethinkDB, all rights reserved.
#include "clustering/administration/servers/server_status.hpp"

#include "clustering/administration/admin_op_exc.hpp"
#include "clustering/administration/datum_adapter.hpp"
#include "clustering/administration/metadata.hpp"
#include "clustering/administration/main/watchable_fields.hpp"
#include "clustering/administration/servers/config_client.hpp"

ql::datum_t convert_ip_to_datum(const ip_address_t &ip) {
    return ql::datum_t(datum_string_t(ip.to_string()));
}

ql::datum_t convert_host_and_port_to_datum(const host_and_port_t &x) {
    ql::datum_object_builder_t builder;
    builder.overwrite("host", ql::datum_t(datum_string_t(x.host())));
    builder.overwrite("port", convert_port_to_datum(x.port().value()));
    return std::move(builder).to_datum();
}

server_status_artificial_table_backend_t::server_status_artificial_table_backend_t(
        rdb_context_t *rdb_context,
        lifetime_t<name_resolver_t const &> name_resolver,
        watchable_map_t<peer_id_t, cluster_directory_metadata_t> *_directory,
        server_config_client_t *_server_config_client,
        admin_identifier_format_t _admin_format)
    : common_server_artificial_table_backend_t(
        name_string_t::guarantee_valid("server_status"),
        rdb_context,
        name_resolver,
        _server_config_client,
        _directory),
      server_config_client(_server_config_client),
      admin_format(_admin_format),
      directory_subs(_directory,
        [&](const peer_id_t &peer, const cluster_directory_metadata_t *metadata) {
            if (metadata == nullptr) {
                connect_times.erase(peer);
            } else {
                if (connect_times.count(peer) == 0) {
                    connect_times.insert(std::make_pair(peer, current_microtime()));
                }
            }
        }, initial_call_t::YES) {
}

server_status_artificial_table_backend_t::~server_status_artificial_table_backend_t() {
    begin_changefeed_destruction();
}

bool server_status_artificial_table_backend_t::format_row(
        UNUSED auth::user_context_t const &user_context,
        server_id_t const & server_id,
        peer_id_t const & peer_id,
        cluster_directory_metadata_t const & metadata,
        UNUSED signal_t *interruptor_on_home,
        ql::datum_t *row_out,
        UNUSED admin_err_t *error_out) {
    ql::datum_object_builder_t builder;
    builder.overwrite("name",
        convert_name_to_datum(metadata.server_config.config.name));
    builder.overwrite("id", convert_server_id_to_datum(server_id));

    ql::datum_object_builder_t proc_builder;
    proc_builder.overwrite("time_started",
        convert_microtime_to_datum(metadata.proc.time_started));
    proc_builder.overwrite("version",
        ql::datum_t(datum_string_t(metadata.proc.version)));
    proc_builder.overwrite("pid", ql::datum_t(static_cast<double>(metadata.proc.pid)));
    proc_builder.overwrite("argv",
        convert_vector_to_datum<std::string>(
            &convert_string_to_datum,
            metadata.proc.argv));
    proc_builder.overwrite("cache_size_mb", ql::datum_t(
        static_cast<double>(metadata.actual_cache_size_bytes) / MEGABYTE));
    builder.overwrite("process", std::move(proc_builder).to_datum());

    ASSERT_NO_CORO_WAITING;
    server_config_client->assert_thread();
    const server_connectivity_t& connect =
        server_config_client->get_server_connectivity();
    ql::datum_object_builder_t net_builder;
    ql::datum_object_builder_t server_connect_builder;

    for (auto pair : connect.all_servers) {
        ql::datum_t server_name_or_uuid;
        if (!convert_connected_server_id_to_datum(
                pair.first,
                admin_format,
                server_config_client,
                &server_name_or_uuid,
                nullptr)) {
            // If we can't resolve the name, use uuid
            server_name_or_uuid = ql::datum_t(
                datum_string_t(pair.first.print()));
        }
        if (server_id != pair.first) {
            bool is_connected = false;
            const auto server_it = connect.connected_to.find(server_id);
            if (server_it != connect.connected_to.end() &&
                  server_it->second.count(pair.first) > 0) {
                is_connected = true;
            }
            bool conflict =
                server_connect_builder.add(
                    server_name_or_uuid.as_str(),
                    ql::datum_t::boolean(is_connected));
            if (admin_format == admin_identifier_format_t::uuid) {
                guarantee(!conflict);
            } else {
                if (conflict) {
                    std::string duplicate_name = server_name_or_uuid.as_str().to_std()
                        + "__conflict__"
                        + pair.first.print();
                    std::replace(duplicate_name.begin(), duplicate_name.end(), '-', '_');
                    bool retry_conflict =
                        server_connect_builder.add(
                            datum_string_t(duplicate_name),
                            ql::datum_t::boolean(is_connected));
                    rcheck_toplevel(!retry_conflict,
                                    ql::base_exc_t::INTERNAL,
                                    "A server already has the duplicate name "
                                    + duplicate_name);
                }
            }
        }
    }
    net_builder.overwrite("connected_to",
                          std::move(server_connect_builder).to_datum());
    net_builder.overwrite("hostname",
        ql::datum_t(datum_string_t(metadata.proc.hostname)));
    net_builder.overwrite("cluster_port",
        convert_port_to_datum(metadata.proc.cluster_port));
    net_builder.overwrite("reql_port",
        convert_port_to_datum(metadata.proc.reql_port));
    net_builder.overwrite("http_admin_port",
        static_cast<bool>(metadata.proc.http_admin_port)
            ? convert_port_to_datum(*metadata.proc.http_admin_port)
            : ql::datum_t("<no http admin>"));
    net_builder.overwrite("canonical_addresses",
        convert_set_to_datum<host_and_port_t>(
            &convert_host_and_port_to_datum,
            metadata.proc.canonical_addresses));
    {
        auto conn_time_it = connect_times.find(peer_id);
        if (conn_time_it != connect_times.end()) {
            net_builder.overwrite("time_connected",
                convert_microtime_to_datum(conn_time_it->second));
        } else {
            net_builder.overwrite("time_connected",
                convert_microtime_to_datum(current_microtime()));
        }
    }
    builder.overwrite("network", std::move(net_builder).to_datum());

    *row_out = std::move(builder).to_datum();
    return true;
}

bool server_status_artificial_table_backend_t::write_row(
        UNUSED auth::user_context_t const &user_context,
        UNUSED ql::datum_t primary_key,
        UNUSED bool pkey_was_autogenerated,
        UNUSED ql::datum_t *new_value_inout,
        UNUSED signal_t *interruptor_on_caller,
        admin_err_t *error_out) {
    *error_out = admin_err_t{
        "It's illegal to write to the `rethinkdb.server_status` table.",
        query_state_t::FAILED};
    return false;
}

