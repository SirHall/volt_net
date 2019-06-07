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

        unsigned char prot_id() { return 0; }

        void initialize() {}

        void send_msg(volt::message const &m)
        {
            send(con_fd, m.get_buffer(), m.get_buffer_len(), 0);
        }

        bool new_msgs()
        {
            poll(&poll_fd, 1, 0);
            return poll_fd.revents & (POLLWRBAND | POLLOUT);
        }

        std::vector<volt::message> *recieve_msg()
        {
            auto msgs = new std::vector<volt::message>();
            while (new_msgs())
            {

                auto buff = std::vector<net_word>(volt::buffer_size, 0);

                recv(con_fd, buff.data(), volt::buffer_size, 0);

                volt::net_tag tag = ((net_tag *)buff.data())[0];
                auto          msg = volt::message(tag);
                msg.swap(buff);
                msgs->push_back(msg);
                // Do not use in_buff anymore!
            }
            return msgs;
        }
    };
} // namespace volt::protocol

#endif