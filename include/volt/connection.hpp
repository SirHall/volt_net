#pragma once
#ifndef connection_hpp
#define connection_hpp
#include <netinet/in.h>
#include <poll.h>

#include "volt/defs.hpp"
#include "volt/protocols/protocol.hpp"
#include <map>
#include <memory>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

namespace volt
{
    class message;

    class connection
    {
      private:
        std::map<volt::prot_identifier,
                 std::unique_ptr<volt::protocol::protocol>>
            protocols;

      public:
        // Creates a new connection given the TCP file descriptor
        connection() {}

        ~connection() {}

        // Attempts to connect to a listening server, returns state
        // TODO: CLEANUP
        static int server_connect(int &socket_fd, sockaddr_in &addr)
        {
            // auto addr = std::make_unique<sockaddr_in>();
            // int      socket_fd = 0;
            uint16_t port = 64423;
            // pollfd   poll_fd;

            // Get the socket file descriptor for the new socket
            socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            // Setup polling so we can check for new connections
            // poll_fd.fd     = socket_fd;
            // poll_fd.events = POLLOUT | POLLWRBAND;

            // Addr settings
            addr.sin_family      = AF_INET; // TCP
            addr.sin_port        = htons(port);
            addr.sin_addr.s_addr = INADDR_ANY;

            return connect(socket_fd, (sockaddr *)&addr, sizeof(addr));
        }

        void add_protocol(std::unique_ptr<volt::protocol::protocol> prot)
        {
            protocols.insert_or_assign(prot->prot_id(), std::move(prot));
        }

        bool supports_protocol(volt::prot_identifier prot_id)
        {
            return protocols.count(prot_id) > 0;
        }

        // TODO: Find some clean way to specify which protocol to use
        void send_message(message &m, volt::prot_identifier prot_id)
        {
            protocols[prot_id]->send_msg(m);
        }

        std::unique_ptr<std::vector<std::unique_ptr<volt::message>>>
            recieve_messages(volt::prot_identifier prot_id)
        {
            return std::move(protocols[prot_id]->recieve_msg());
        }
    };
} // namespace volt

#endif
