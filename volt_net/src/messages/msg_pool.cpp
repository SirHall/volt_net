#include "volt/messages/msg_pool.hpp"

static std::vector<volt::message_ptr> msg_vec;
static std::mutex                     mut;
static constexpr unsigned int         max_messages = 50;

volt::message_ptr volt::msg_pool::get_message()
{
    {
        std::lock_guard guard(mut);
        if (msg_vec.size() > 0)
        {
            volt::message_ptr msg = std::move(msg_vec.back());
            msg_vec.pop_back();
            return msg;
        }
    }
    return std::move(volt::make_message());
}

void volt::msg_pool::return_message(volt::message_ptr msg)
{
    std::lock_guard guard(mut);
    if (msg_vec.size() >= max_messages)
        msg_vec.push_back(std::move(msg));
    else
        msg.~unique_ptr();
    // Otherwise, just destroy the message
}