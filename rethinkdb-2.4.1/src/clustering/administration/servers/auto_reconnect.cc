// Copyright 2010-2012 RethinkDB, all rights reserved.
#include "clustering/administration/servers/auto_reconnect.hpp"

#include "arch/timing.hpp"
#include "clustering/administration/servers/config_client.hpp"
#include "concurrency/exponential_backoff.hpp"
#include "concurrency/wait_any.hpp"
#include "logger.hpp"

auto_reconnector_t::auto_reconnector_t(
        connectivity_cluster_t *connectivity_cluster_,
        connectivity_cluster_t::run_t *connectivity_cluster_run_,
        server_config_client_t *server_config_client_,
        const int join_delay_secs_,
        const int give_up_ms_) :
    connectivity_cluster(connectivity_cluster_),
    connectivity_cluster_run(connectivity_cluster_run_),
    server_config_client(server_config_client_),
    join_delay_secs(join_delay_secs_),
    give_up_ms(give_up_ms_),
    server_id_subs(
        server_config_client->get_peer_to_server_map(),
        std::bind(&auto_reconnector_t::on_connect_or_disconnect, this, ph::_1),
        initial_call_t::NO),
    connection_subs(
        connectivity_cluster->get_connections(),
        std::bind(&auto_reconnector_t::on_connect_or_disconnect, this, ph::_1),
        initial_call_t::YES)
    { }

void auto_reconnector_t::on_connect_or_disconnect(const peer_id_t &peer_id) {
    optional<server_id_t> server_id =
        server_config_client->get_peer_to_server_map()->get_key(peer_id);
    optional<connectivity_cluster_t::connection_pair_t> conn =
        connectivity_cluster->get_connections()->get_key(peer_id);
    // The criterion for when we consider a peer connected must be consistent with
    // what we define in `initial_joiner_t::on_connection_change`, or else we might
    // never start the `try_reconnect` routine for an initial join peer.
    if (conn.has_value()) {
        // We never reconnect *to* proxies. If we are a full server, the proxy is going
        // to connect to us instead.
        if (!conn->first->get_server_id().is_proxy()) {
            addresses[conn->first->get_server_id()] = conn->first->get_peer_address();
            server_ids[peer_id] = conn->first->get_server_id();
        }
    } else if (!server_id.has_value() && !conn.has_value()) {
        auto it = server_ids.find(peer_id);
        if (it != server_ids.end()) {
            coro_t::spawn_sometime(std::bind(&auto_reconnector_t::try_reconnect, this,
                it->second, drainer.lock()));
            server_ids.erase(it);
        }
    }
}

void auto_reconnector_t::try_reconnect(const server_id_t &server,
                                       auto_drainer_t::lock_t keepalive) {
    peer_address_t last_known_address;
    auto it = addresses.find(server);
    guarantee(it != addresses.end());
    last_known_address = it->second;

    signal_timer_t give_up_timer;
    give_up_timer.start(give_up_ms);

    cond_t reconnected;
    watchable_map_t<server_id_t, peer_id_t>::key_subs_t subs(
        server_config_client->get_server_to_peer_map(),
        server,
        [&](const peer_id_t *pid) {
            if (pid != nullptr) {
                reconnected.pulse_if_not_already_pulsed();
                // Add the server back into `server_ids` so that `try_reconnect` is going
                // to be started again in `on_connect_or_disconnect` when the connection
                // gets dropped, even if we never get an entry in the
                // `connectivity_cluster`'s connections map that
                // `on_connect_or_disconnect` is listening on.
                server_ids[*pid] = server;
            }
        },
        initial_call_t::YES);

    cond_t join_failed;
    wait_any_t interruptor(
        &reconnected, &give_up_timer, keepalive.get_drain_signal(), &join_failed);
    exponential_backoff_t backoff(50, 15 * 1000);
    try {
        // These can be safely passed into the coroutine below.
        // They will be reset to `nullptr` by the assignment_sentry_ts when this function
        // call ends.
        std::shared_ptr<cond_t *> join_failed_out(new cond_t *(nullptr));
        std::shared_ptr<peer_address_t *> last_known_address_out(
            new peer_address_t *(nullptr));
        assignment_sentry_t<cond_t *> join_failed_out_assignment(
            join_failed_out.get(), &join_failed);
        assignment_sentry_t<peer_address_t *> last_known_address_out_assignment(
            last_known_address_out.get(), &last_known_address);

        while (!interruptor.is_pulsed()) {
            // This coroutine can keep running even after this function has returned
            // or the auto_reconnector_t was destructed. So we must pass everything
            // we need by value.

            coro_t::spawn_now_dangerously(
                [this, last_known_address, server, join_failed_out, last_known_address_out]() {
                auto connectivity_cluster_run_local = connectivity_cluster_run;
                auto join_delay_secs_local = join_delay_secs;

                join_results_t results =
                    connectivity_cluster_run_local->join_blocking(
                        last_known_address, r_nullopt, make_optional(server),
                        join_delay_secs_local,
                        auto_drainer_t::lock_t(&connectivity_cluster_run_local->drainer));

                // Check if the `try_reconnect` function that spawned us is still alive
                if (*join_failed_out != nullptr && *last_known_address_out != nullptr) {
                    ASSERT_NO_CORO_WAITING;

                    for (auto r = results.begin(); r != results.end(); ++r) {
                        if (r->second != join_result_t::PERMANENT_ERROR) continue;
                        (*last_known_address_out)->erase_ip(r->first);
                        if (last_known_address.ips().empty()) {
                            logNTC("Unrecoverable connection error to remote server: %s",
                                server.print().c_str());
                            (*join_failed_out)->pulse_if_not_already_pulsed();
                        }
                    }
                }
            });

            backoff.failure(&interruptor);
        }
    } catch (const interrupted_exc_t &) {
        /* ignore; this is how we escape the loop */
    }
}

