#pragma once
#ifndef VOLT_NET_EVENT_TYPES_E_RECEIVED_MSG_HPP
#define VOLT_NET_EVENT_TYPES_E_RECEIVED_MSG_HPP

#include "volt/net/messages/msg_reader.hpp"
#include "volt/net/volt_net_defs.hpp"

namespace volt::net
{
    class e_received_msg
    {
    private:
        message_ptr msg;

    public:
        e_received_msg(message_ptr message);
        e_received_msg(const e_received_msg &other) = delete;
        e_received_msg &operator=(const e_received_msg &other) = delete;
        e_received_msg(e_received_msg &&other);
        e_received_msg &operator=(e_received_msg &&other);
        ~e_received_msg();

        reader_ptr get_reader();
    };
} // namespace volt::net
#endif
