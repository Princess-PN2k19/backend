// Copyright 2010-2014 RethinkDB, all rights reserved.
#include "clustering/administration/issues/issues_backend.hpp"

#include "clustering/administration/admin_op_exc.hpp"
#include "clustering/administration/datum_adapter.hpp"
#include "clustering/administration/metadata.hpp"
#include "clustering/administration/main/watchable_fields.hpp"
#include "concurrency/cross_thread_signal.hpp"

issues_artificial_table_backend_t::issues_artificial_table_backend_t(
        rdb_context_t *rdb_context,
        lifetime_t<name_resolver_t const &> name_resolver,
        mailbox_manager_t *mailbox_manager,
        std::shared_ptr<semilattice_read_view_t<cluster_semilattice_metadata_t> >
            _cluster_sl_view,
        watchable_map_t<peer_id_t, cluster_directory_metadata_t> *directory_view,
        server_config_client_t *_server_config_client,
        table_meta_client_t *_table_meta_client,
        namespace_repo_t *_namespace_repo,
        admin_identifier_format_t _identifier_format)
    : timer_cfeed_artificial_table_backend_t(
        name_string_t::guarantee_valid("current_issues"), rdb_context, name_resolver),
      identifier_format(_identifier_format),
      cluster_sl_view(_cluster_sl_view),
      server_config_client(_server_config_client),
      table_meta_client(_table_meta_client),
      local_issue_client(mailbox_manager, directory_view),
      name_collision_issue_tracker(
        server_config_client, cluster_sl_view, table_meta_client),
      table_issue_tracker(server_config_client, table_meta_client, _namespace_repo),
      outdated_index_issue_tracker(table_meta_client),
      non_transitive_issue_tracker(_server_config_client)  {
    trackers.insert(&local_issue_client);
    trackers.insert(&name_collision_issue_tracker);
    trackers.insert(&table_issue_tracker);
    trackers.insert(&outdated_index_issue_tracker);
    trackers.insert(&non_transitive_issue_tracker);
}

issues_artificial_table_backend_t::~issues_artificial_table_backend_t() {
    begin_changefeed_destruction();
}

std::string issues_artificial_table_backend_t::get_primary_key_name() {
    return "id";
}

bool issues_artificial_table_backend_t::read_all_rows_as_vector(
        UNUSED auth::user_context_t const &user_context,
        signal_t *interruptor,
        std::vector<ql::datum_t> *rows_out,
        UNUSED admin_err_t *error_out) {
    cross_thread_signal_t ct_interruptor(interruptor, home_thread());
    on_thread_t rethreader(home_thread());
    rows_out->clear();

    cluster_semilattice_metadata_t metadata = cluster_sl_view->get();
    for (auto const &tracker : trackers) {
        for (auto const &issue : tracker->get_issues(&ct_interruptor)) {
            ql::datum_t row;
            bool still_valid = issue->to_datum(cluster_sl_view->get(),
                server_config_client, table_meta_client, identifier_format, &row);
            if (!still_valid) {
                /* Based on `metadata`, the issue decided it is no longer relevant. */
                continue;
            }
            rows_out->push_back(row);
        }
    }

    if (ct_interruptor.is_pulsed()) {
        throw interrupted_exc_t();
    }

    return true;
}

bool issues_artificial_table_backend_t::read_row(
        UNUSED auth::user_context_t const &user_context,
        ql::datum_t primary_key,
        signal_t *interruptor,
        ql::datum_t *row_out,
        UNUSED admin_err_t *error_out) {
    cross_thread_signal_t ct_interruptor(interruptor, home_thread());
    on_thread_t rethreader(home_thread());
    *row_out = ql::datum_t();

    uuid_u issue_id;
    admin_err_t dummy_error;
    if (convert_uuid_from_datum(primary_key, &issue_id, &dummy_error)) {
        std::vector<scoped_ptr_t<issue_t> > issues = all_issues(&ct_interruptor);

        for (auto const &issue : issues) {
            if (issue->get_id() == issue_id) {
                ql::datum_t row;
                bool still_valid = issue->to_datum(cluster_sl_view->get(),
                    server_config_client, table_meta_client, identifier_format, &row);
                if (still_valid) {
                    *row_out = row;
                }
                break;
            }
        }
    }
    return true;
}

std::vector<scoped_ptr_t<issue_t> > issues_artificial_table_backend_t::all_issues(
        signal_t *interruptor) const {
    std::vector<scoped_ptr_t<issue_t> > res;

    for (auto const &tracker : trackers) {
        std::vector<scoped_ptr_t<issue_t> > issues = tracker->get_issues(interruptor);
        std::move(issues.begin(), issues.end(), std::back_inserter(res));
    }

    return res;
}

bool issues_artificial_table_backend_t::write_row(
        UNUSED auth::user_context_t const &user_context,
        UNUSED ql::datum_t primary_key,
        UNUSED bool pkey_was_autogenerated,
        UNUSED ql::datum_t *new_value_inout,
        UNUSED signal_t *interruptor,
        admin_err_t *error_out) {
    *error_out = admin_err_t{
        "It's illegal to write to the `rethinkdb.current_issues` table.",
        query_state_t::FAILED};
    return false;
}

