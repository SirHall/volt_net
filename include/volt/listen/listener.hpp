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

      public:
        listener(std::uint16_t hostport);

        ~listener();

        bool is_open();

        void accept_new_connection();

        void close_listener();
    };
} // namespace volt
#endif