#include "volt/net/messages/msg_rec_pool.hpp"
#include "volt/net/messages/msg_reader.hpp"

#include <iostream>

using namespace volt::net;

msg_rec_pool::msg_rec_pool(
    bool defer, std::function<void(message_ptr)> msg_received_callback)
    : defer_msg_signal(defer), on_msg_received(msg_received_callback)
{
}

msg_rec_pool::~msg_rec_pool() {}

void msg_rec_pool::submit_message(message_ptr msg)
{
    if (defer_msg_signal)
    {
        std::lock_guard lock(mut);
        rec_msg_vec.push(std::move(msg));
    }
    else
    {
        // volt::event::global_event<reader_ptr>::call_event(
        //     make_reader(std::move(msg)));
        this->on_msg_received(std::move(msg));
    }
}

void msg_rec_pool::notify_listeners()
{
    if (!defer_msg_signal)
        return;
    while (true)
    {
        std::lock_guard lock(mut);
        if (rec_msg_vec.size() == 0)
            break;
        auto msg = std::move(rec_msg_vec.front());
        rec_msg_vec.pop();
        lock.~lock_guard();
        // volt::event::global_event<reader_ptr>::call_event(
        //     make_reader(std::move(msg)));
        this->on_msg_received(std::move(msg));
    }
}