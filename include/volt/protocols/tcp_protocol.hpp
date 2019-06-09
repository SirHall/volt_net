#pragma once
#ifndef tcp_protocol_hpp
#define tcp_protocol_hpp

#include "volt/defs.hpp"
#include "volt/message.hpp"
#include "volt/protocols/protocol.hpp"
#include <poll.h>

namespace volt::protocol
{
    class tcp_protocol : public protocol
    {
      private:
        const sockaddr *addr;
        const socklen_t len;
        const int       con_fd;
        pollfd          poll_fd;

      public:
        tcp_protocol(sockaddr *socket, socklen_t length,
                     int con_file_descriptor)
            : addr(socket), len(length), con_fd(con_file_descriptor)
        {
            // Setup polling so we can check for new messages
            poll_fd.fd     = con_fd;
            poll_fd.events = POLLOUT | POLLWRBAND;
        }

        ~tcp_protocol() { delete addr; };

        volt::prot_identifier prot_id() { return 0; }

        void initialize() {}

        void send_msg(volt::message const &m)
        {
            send(con_fd, m.get_buffer(), m.get_buffer_len(), 0);
        }

        bool new_msgs()
        {
            poll(&poll_fd, 1, 0);
            return (poll_fd.revents & (POLLWRBAND | POLLOUT)) > 0;
        }

        std::unique_ptr<std::vector<std::unique_ptr<volt::message>>>
            recieve_msg()
        {
            auto msgs =
                std::unique_ptr<std::vector<std::unique_ptr<volt::message>>>(
                    new std::vector<std::unique_ptr<volt::message>>());

            int i = 0;

            while (new_msgs())
            {

                auto msg = std::make_unique<volt::message>();
                msg->resize(volt::buffer_size);
                recv(con_fd, msg->data(), volt::buffer_mem_size, 0);
                msgs->push_back(std::move(msg));

                if (i > 5)
                {
                    std::cout << "i > 0" << std::endl;
                    break;
                }
            }
            return msgs;
        }
    };
} // namespace volt::protocol

#endif