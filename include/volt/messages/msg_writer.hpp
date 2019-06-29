#pragma once
#ifndef msg_writer_hpp
#define msg_writer_hpp

#include "volt/message.hpp"
#include "volt/messages/msg_pool.hpp"
#include "volt/serialization.hpp"
#include <memory>

namespace volt
{
    class msg_writer
    {
      private:
        volt::message_ptr msg;

      public:
        msg_writer(volt::message_ptr message) { msg = std::move(message); }

        msg_writer() { msg = std::move(volt::msg_pool::get_message()); }

        ~msg_writer()
        {
            volt::msg_pool::return_message(std::move(msg));
            msg->resize(0);
        }

        template <typename T>
        void write_msg(T const &instance)
        {
            volt::serialize::write_into<T>(instance, msg);
        }

        volt::message_ptr &get_msg() { return std::ref(msg); }
    };
} // namespace volt

#endif
