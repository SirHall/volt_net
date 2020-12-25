#pragma once
#ifndef VOLT_NET_MESSAGES_MSG_POOL_HPP
#define VOLT_NET_MESSAGES_MSG_POOL_HPP

#include "volt/net/volt_net_defs.hpp"
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace volt::net::msg_pool
{
    message_ptr get_message();

    void return_message(std::vector<net_word> *msg);
} // namespace volt::net::msg_pool

#endif
