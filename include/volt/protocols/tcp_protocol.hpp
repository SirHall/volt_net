#pragma once
#ifndef tcp_protocol_hpp
#define tcp_protocol_hpp

#include "volt/defs.hpp"
#include "volt/message.hpp"
#include "volt/protocols/protocol.hpp"
#include <cstdint>
#include <poll.h>

namespace volt::protocol
{
    class tcp_protocol : public protocol
    {
      private:
        const sockaddr *            addr;
        const socklen_t             len;
        const int                   con_fd;
        pollfd                      poll_fd;
        std::vector<volt::net_word> buff;

        volt::message_iter iter;

        void recieve_next_buf()
        {
            recv(con_fd, buff.data(), volt::max_buffer_size, 0);
            iter = buff.begin();
        }

        volt::net_word get_next_byte()
        {
            if (iter == buff.end())
                recieve_next_buf();
            auto val = *iter;
            iter++;
            return val;
        }

        volt::net_word *get_next_bytes(std::size_t count)
        {
            volt::net_word *alloc_mem = (volt::net_word *)malloc(count);
            for (std::size_t i = 0; i < count; i++)
                alloc_mem[i] = get_next_byte();
            return alloc_mem;
        }

      public:
        tcp_protocol(sockaddr *socket, socklen_t length,
                     int con_file_descriptor)
            : addr(socket), len(length), con_fd(con_file_descriptor)
        {
            // Setup polling so we can check for new messages
            poll_fd.fd     = con_fd;
            poll_fd.events = POLLOUT | POLLWRBAND;
            buff.resize(volt::max_buffer_size);
        }

        ~tcp_protocol() { delete addr; }

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

        volt::message_ptr recieve_msg()
        {
            volt::message_ptr msg  = volt::message_ptr(new volt::message());
            auto              iter = msg->begin();

            // Get message length
            volt::buffer_size msg_len = 0;
            volt::deserialize::unsafe::read_into_raw(msg_len, &*iter);

            // current_size == msg_len must be true
            volt::buffer_size current_size = 0;
            while (true)
            {
                net_word b = get_next_byte();
                current_size++;

                if (b == volt::escape_val) // This is an escape sequence
                {
                    net_word next = get_next_byte();
                    current_size++;
                    // This is the original character (escap evalue)
                    if (next == volt::msg_origi_char) {}
                    // This is the end of the message
                    if (next == volt::msg_end_escaped)
                    {
                        break;
                    }
                }
                msg->push_back(b);
            }
            return msg;
        }
    };
} // namespace volt::protocol

#endif
