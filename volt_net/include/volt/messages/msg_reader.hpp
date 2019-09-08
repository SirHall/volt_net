#pragma once
#ifndef msg_reader_hpp
#define msg_reader_hpp

#include "volt/messages/msg_pool.hpp"
#include "volt/serialization/net_serialize.hpp"
#include "volt/serialization/serialization_ext.hpp"
#include "volt/volt_defs.hpp"
#include <memory>

namespace volt
{
    class msg_reader
    {
      private:
        volt::message_ptr  msg;
        volt::message_iter iter;

      public:
        msg_reader(volt::message_ptr message);

        msg_reader();

        ~msg_reader();

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

    typedef std::unique_ptr<msg_reader> reader_ptr;

    reader_ptr make_reader();

    reader_ptr make_reader(message_ptr msg);
} // namespace volt

#endif