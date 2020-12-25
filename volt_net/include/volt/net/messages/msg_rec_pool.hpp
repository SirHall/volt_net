#pragma once
#ifndef VOLT_NET_MESSAGES_MSG_REC_POOL_HPP
#define VOLT_NET_MESSAGES_MSG_REC_POOL_HPP

// #include "volt/event/global_event.hpp"
#include "volt/net/volt_net_defs.hpp"
#include <functional>
#include <mutex>
#include <queue>

namespace volt::net
{
    class msg_rec_pool
    {
        std::queue<message_ptr>                rec_msg_vec;
        std::mutex                             mut;
        const bool                             defer_msg_signal = false;
        const std::function<void(message_ptr)> on_msg_received;

    private:
    public:
        msg_rec_pool(bool                             defer,
                     std::function<void(message_ptr)> msg_received_callback);
        ~msg_rec_pool();
        void submit_message(message_ptr msg);

        void notify_listeners();
    };
} // namespace volt::net

#endif
