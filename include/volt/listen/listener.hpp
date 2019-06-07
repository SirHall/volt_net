#pragma once
#ifndef listener_hpp
#define listener_hpp

#include "volt/connection.hpp"
#include "volt/protocols/protocol.hpp"
#include "volt/protocols/tcp_protocol.hpp"
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace volt::listener
{
    class listener
    {
      private:
        sockaddr_in addr;
        int         socket_fd = 0;
        uint16_t    port      = 0;
        pollfd      poll_fd;
        int         backlog = 10;

        void open_socket()
        {
            // Get the socket file descriptor for the new socket
            socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            // Setup polling so we can check for new connections
            poll_fd.fd     = socket_fd;
            poll_fd.events = POLLOUT | POLLWRBAND;

            // Addr settings
            addr.sin_family      = AF_INET; // TCP
            addr.sin_port        = htons(port);
            addr.sin_addr.s_addr = INADDR_ANY;

            // Bind this file descriptor to a port
            if (bind(socket_fd, (sockaddr *)&addr, sizeof(addr)) < 0)
            {
                // TODO: Handle error
            }

            if (listen(socket_fd, backlog) == -1)
            {
                // TODO: Handle error
            }
        }

      public:
        listener() { open_socket(); }
        ~listener() {}

        // Check for any new connection requests
        // returns: true = new connection waiting
        bool new_connection()
        {
            poll(&poll_fd, 1, 0);
            return poll_fd.revents & (POLLWRBAND | POLLOUT);
        }

        // Accepts the new connection
        // return: new connection file descriptor. -1 on failure
        std::vector<connection> *accept_new_connection()
        {
            auto new_cons = new std::vector<connection>();

            while (new_connection())
            {
                sockaddr_in new_addr;
                socklen_t   len;

                int conn_fd = accept(socket_fd, (sockaddr *)&new_addr, &len);

                connection new_con = connection();
                // new_con

                // TODO: Setup the connection
                // auto tcp_prot = volt::protocol::tcp_protocol();
            }
        }
    };
} // namespace volt::listener
#endif