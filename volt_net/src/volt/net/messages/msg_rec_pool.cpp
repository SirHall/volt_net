#include "volt/net/messages/msg_rec_pool.hpp"
#include "volt/net/messages/msg_reader.hpp"

using namespace volt::net;

msg_rec_pool::msg_rec_pool(
    bool defer, std::function<void(con_id, message_ptr)> msg_received_callback)
    : defer_msg_signal(defer), on_msg_received(msg_received_callback)
{
}

msg_rec_pool::~msg_rec_pool() {}

void msg_rec_pool::submit_message(con_id id, message_ptr msg)
{
    if (defer_msg_signal)
    {
        std::lock_guard lock(mut);
        rec_msg_vec.push(msg);
        rec_msg_ids.push(id);
    }
    else
    {
        this->on_msg_received(id, msg);
    }
}

void msg_rec_pool::notify_listeners()
{
    if (!defer_msg_signal)
        return;
    while (!rec_msg_vec.empty())
    {
        volt::net::message_ptr msg;
        con_id                 id = 0;
        {
            std::lock_guard lock(mut);
            // I am assuming that these two queue will always remain the same
            // size
            msg = rec_msg_vec.front();
            rec_msg_vec.pop();
            id = rec_msg_ids.front();
            rec_msg_ids.pop();
        }
        this->on_msg_received(id, msg);
    }
}
