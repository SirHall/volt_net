#include "volt/messages/msg_rec_pool.hpp"

static std::queue<volt::message_ptr> rec_msg_vec;
static std::mutex                    mut;
static constexpr bool                defer_msg_signal = false;

void volt::net::submit::submit_message(volt::message_ptr msg)
{
    if (defer_msg_signal)
    {
        std::lock_guard lock(mut);
        rec_msg_vec.push(std::move(msg));
    }
    else
    {
        volt::event::global_event<volt::reader_ptr>::call_event(
            volt::make_reader(std::move(msg)));
    }
}

void volt::net::notify_listeners()
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
        volt::event::global_event<volt::reader_ptr>::call_event(
            volt::make_reader(std::move(msg)));
    }
}