#pragma once
#ifndef msg_reader_hpp
#define msg_reader_hpp

#include "volt/defs.hpp"
#include "volt/message.hpp"
#include "volt/messages/msg_pool.hpp"
#include "volt/serialization.hpp"
#include <memory>

namespace volt
{
    class msg_reader
    {
      private:
        volt::message_ptr  msg;
        volt::message_iter iter;

      public:
        msg_reader(volt::message_ptr message)
        {
            msg  = std::move(message);
            iter = msg->begin();
        }

        msg_reader()
        {
            msg  = std::move(volt::msg_pool::get_message());
            iter = msg->begin();
        }

        ~msg_reader() { volt::msg_pool::return_message(std::move(msg)); }

        template <typename T>
        void read_msg(T &instance)
        {
            volt::deserialize::read_into<T>(iter, instance);
        }

        template <typename T>
        void read_msg_int(T &instance)
        {
            volt::deserialize::read_into_int<T>(iter, instance);
        }
    };
} // namespace volt

#endif