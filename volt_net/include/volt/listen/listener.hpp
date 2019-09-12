#pragma once
#ifndef listener_hpp
#define listener_hpp

#include "volt/net_con.hpp"
#include <atomic>
#include <memory>
#include <mutex>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>

namespace volt
{
    class listener
    {
      private:
        // TODO Change this into the generic sockaddr struct
        std::unique_ptr<sockaddr> addr;
        int                       socket_fd = 0;
        uint16_t                  port      = volt::default_port;
        int                       backlog   = 10;
        std::thread               thr;
        std::atomic_bool          listener_open;

        struct pollfd fds[1];
        int           timeout = 1000;

        void loop();

        int open_socket();

        void accept_new_connection();

      public:
        /**
         * @brief Construct a new listener object that listens for new
         * connections on a desired port.
         *
         * @param hostport The port for the listener to find new connections on
         */
        listener(std::uint16_t hostport);

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
} // namespace volt
#endif