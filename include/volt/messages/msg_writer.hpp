#pragma once
#ifndef msg_writer_hpp
#define msg_writer_hpp

#include "volt/message.hpp"
#include "volt/serialization.hpp"
#include <memory>

namespace volt
{
    class msg_writer
    {
      private:
        volt::message_ptr msg;

      public:
        msg_writer()
        {
            // TODO: Get message instance from central queue
            msg = volt::make_message();

            msg->resize(0);
        }
        ~msg_writer()
        {
            // TODO: Give the message back to the central queue

            // volt::serialize::write_end(msg);
        }

        template <typename T>
        void write_msg(T const &instance)
        {
            volt::serialize::write_into<T>(instance, msg);
        }

        // Temporary
        volt::message_ptr &get_msg() { return std::ref(msg); }
    };
} // namespace volt

#endif
