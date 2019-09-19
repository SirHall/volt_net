#pragma once
#ifndef msg_rec_pool_hpp
#define msg_rec_pool_hpp

#include "volt/event/global_event.hpp"
#include "volt/messages/msg_reader.hpp"
#include "volt/volt_defs.hpp"
#include <mutex>
#include <queue>

namespace volt::net
{
    namespace submit
    {
        void submit_message(volt::message_ptr msg);
    } // namespace submit

    void notify_listeners();
} // namespace volt::net

#endif
