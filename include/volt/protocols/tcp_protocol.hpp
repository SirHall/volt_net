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
        std::vector<volt::net_word> send_buff;

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
            std::cout << "Next byte: " << (int)val << std::endl;
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
            buff.reserve(volt::max_buffer_size);
            send_buff.reserve(volt::max_buffer_size);
            iter = buff.begin();
        }

        ~tcp_protocol() { delete addr; }

        volt::prot_identifier prot_id() { return 0; }

        void initialize() {}

        void send_msg(volt::message_ptr const &m)
        {
            send_buff.resize(0);
            for (std::size_t i = 0; i < m->size(); i++)
            {
                send_buff.push_back(m->at(i));
                if (m->at(i) == volt::escape_val)
                    send_buff.push_back(volt::msg_origi_char);
            }

            send_buff.push_back(volt::escape_val);
            send_buff.push_back(volt::msg_end_escaped);

            std::cout << "Sending:";
            for (std::size_t i = 0; i < send_buff.size(); i++)
                std::cout << " " << (unsigned int)send_buff.at(i);
            std::cout << std::endl;
            send(con_fd, send_buff.data(), send_buff.size(), 0);
        }

        bool new_msgs()
        {
            poll(&poll_fd, 1, 0);
            return (poll_fd.revents & (POLLWRBAND | POLLOUT)) > 0;
        }

        void recieve_msg(volt::message_ptr &msg)
        {
            msg->resize(0);

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
                    // This is the original character (escape value)
                    if (next == volt::msg_origi_char) {}
                    // This is the end of the message
                    if (next == volt::msg_end_escaped)
                        break;
                }
                msg->push_back(b);
            }
        }
    };
} // namespace volt::protocol

#endif
