#pragma once
#ifndef VOLT_NET_NETWORK_HPP
#define VOLT_NET_NETWORK_HPP

#include "volt/net/AsyncStateHandler.hpp"
#include "volt/net/listener.hpp"
#include "volt/net/messages/msg_rec_pool.hpp"
#include "volt/net/net_con.hpp"

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace volt::net
{
    typedef std::lock_guard<std::recursive_mutex> net_lock;

    class network
    {
    private:
        std::atomic_bool                         running;
        std::vector<connection_ptr>              connections;
        std::recursive_mutex                     connections_mut;
        std::shared_ptr<boost::asio::io_context> io;
        std::weak_ptr<network>                   self;
        std::vector<std::thread>                 thr_pool;
        std::atomic<con_id>                      next_id = 0;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
            work;

        connection_ptr null_con =
            std::unique_ptr<net_con>(nullptr); // A null connection

        std::unique_ptr<listener>       net_listener;
        msg_rec_pool                    received_msgs;
        std::function<void(reader_ptr)> new_msg_callback;

        network(std::size_t thread_count, bool defer_received_msgs);

    public:
        static std::shared_ptr<network> create(std::size_t thread_count,
                                               bool        defer_received_msgs);

        network(const network &other) = delete;
        network &operator=(const network &other) = delete;
        network(network &&other)                 = delete;
        network &operator=(network &&other) = delete;

        std::lock_guard<std::recursive_mutex> get_guard();

        void add_connection(std::unique_ptr<net_con> con, net_lock const &lck);

        connection_ptr &get_con(con_id id, net_lock const &lck);

        connection_ptr &get_con_index(std::size_t index, net_lock const &lck);

        std::size_t get_con_count(net_lock const &lck);

        std::shared_ptr<AsyncStateHandler<ServerConnectStatus>>
            server_connect(std::string const address, std::string const port);

        ~network();

        bool is_running();

        void close(net_lock const &lck);

        std::weak_ptr<boost::asio::io_context> get_io();

        void start_listening(std::uint16_t hostport_ipv4,
                             std::uint16_t hostport_ipv6, net_lock const &lck);

        con_id get_next_id();

        void set_new_msg_callback(std::function<void(reader_ptr)> callback);

        void process_new_msgs();

        std::weak_ptr<network> get_net_weak();
    };
} // namespace volt::net

#endif
