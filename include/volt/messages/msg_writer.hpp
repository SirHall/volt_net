#pragma once
#ifndef msg_writer_hpp
#define msg_writer_hpp

#include "volt/messages/msg_pool.hpp"
#include "volt/serialization/net_serialize.hpp"
#include <memory>

namespace volt
{
    class msg_writer
    {
      private:
        volt::message_ptr msg;

      public:
        msg_writer(volt::message_ptr message);

        msg_writer();

        ~msg_writer();

        template <typename T>
        void write_msg(T const &instance)
        {
            volt::serialize::write_into<T>(instance, msg);
        }

        volt::message_ptr &get_msg();
    };

    typedef std::unique_ptr<msg_writer> writer_ptr;

    writer_ptr make_writer();
    writer_ptr make_writer(message_ptr msg);

} // namespace volt

#endif
