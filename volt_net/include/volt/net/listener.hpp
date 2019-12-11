#pragma once
#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "volt/net/net_con.hpp"

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace volt::net
{
    /**
     * @brief Listens for new connection requests in the background and
     * contructs new net_con intances upon a new connection
     *
     */
    class listener
    {
    private:
        std::shared_ptr<boost::asio::io_context> io;
        tcp::acceptor                            ipv4_acceptor;
        tcp::acceptor                            ipv6_acceptor;

        std::function<void(tcp::socket)> new_con_callback;
        // I create two new sockets so I don't have to worry about
        // syncronization between the ipv4 and ipv6 listeners
        tcp::socket new_sock_ipv4;
        tcp::socket new_sock_ipv6;

        std::atomic_bool listener_open;

        void accept_v4(const boost::system::error_code &err);
        void accept_v6(const boost::system::error_code &err);
        void accept_new_connection(const boost::system::error_code &err,
                                   tcp::socket                      socket);

    public:
        /**
         * @brief Construct a new listener object that listens for new
         * connections on a desired port.
         *
         * @param hostport The port for the listener to find new connections on
         */
        listener(std::uint16_t hostport_ipv4, std::uint16_t hostport_ipv6,
                 std::function<void(tcp::socket)>         callback,
                 std::shared_ptr<boost::asio::io_context> io_context);

        /**
         * @brief Destroy the listener object
         *
         */
        ~listener();

        /**
         * @brief Returns the status of the listener
         *
         * @return true The listener is currently listening for new connections
         * @return false The listener is currently offline
         */
        bool is_open();

        /**
         * @brief Closes the listener and sets it to an offline state
         *
         */
        void close_listener();
    };
} // namespace volt::net
#endif