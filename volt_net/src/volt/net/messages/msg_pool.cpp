#include "volt/net/messages/msg_pool.hpp"

using namespace volt::net;

static std::vector<message_ptr> msg_vec;
static std::mutex               mut;
static constexpr unsigned int   max_messages = 50;

message_ptr volt::net::msg_pool::get_message()
{
    {
        std::lock_guard guard(mut);
        if (msg_vec.size() > 0)
        {
            message_ptr msg = std::move(msg_vec.back());
            msg_vec.pop_back();
            return msg;
        }
    }
    return make_message();
}

void volt::net::msg_pool::return_message(std::vector<net_word> *msg)
{
    std::lock_guard guard(mut);
    if (msg_vec.size() >= max_messages)
        msg_vec.push_back(std::shared_ptr<std::vector<net_word>>(msg));
    else
        delete msg;
    // Otherwise, just destroy the message
}