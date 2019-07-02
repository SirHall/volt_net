#pragma once
#ifndef msg_pool_hpp
#define msg_pool_hpp

#include "volt/defs.hpp"
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

static std::vector<volt::message_ptr> msg_vec;
static std::mutex                     _mut;
static constexpr unsigned int         max_messages = 50;

namespace volt::msg_pool
{

    volt::message_ptr get_message()
    {
        {
            std::lock_guard guard(_mut);
            if (msg_vec.size() > 0)
            {
                volt::message_ptr msg = std::move(msg_vec.back());
                msg_vec.pop_back();
                return msg;
            }
        }
        return std::move(volt::make_message());
    }

    void return_message(volt::message_ptr msg)
    {
        std::lock_guard guard(_mut);
        if (msg_vec.size() >= max_messages)
            msg_vec.push_back(std::move(msg));
        // Otherwise, just destroy the message
    }
} // namespace volt::msg_pool

#endif
