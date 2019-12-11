#pragma once
#ifndef msg_pool_hpp
#define msg_pool_hpp

#include "volt/net/volt_net_defs.hpp"
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace volt::net::msg_pool
{
    message_ptr get_message();

    void return_message(message_ptr msg);
} // namespace volt::net::msg_pool

#endif
