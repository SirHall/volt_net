#pragma once
#ifndef message_hpp
#define message_hpp

#include "volt/defs.hpp"
#include "volt/serialization.hpp"
#include "volt/serialization_ext.hpp"
#include <vector>

namespace volt
{
    class message : public std::vector<net_word>
    {
      private:
        volt::message_iter iter;
        // const volt::net_tag tag;

      public:
        // message(volt::net_tag msg_tag) : tag(msg_tag)
        // {
        //     this->write<volt::net_tag>(msg_tag);
        //     this->reset_iterator();
        // }

        message() { this->reset_iterator(); }

        ~message() = default;

        void reset_iterator() { iter = begin(); }

        void *      get_buffer() const { return (void *)&(*this->begin()); }
        std::size_t get_buffer_len() const { return this->size(); }

        template <typename T>
        void write(T const &v)
        {
            volt::serialize::write_into<T>(v, *this);
        }

        template <typename T>
        void write_array(std::vector<T> &array)
        {
            volt::serialize::write_into_array<T>(array, *this);
        }

        template <typename T>
        T *read_new()
        {
            return volt::deserialize::read_new<T>(this->iter);
        }

        template <typename T>
        void read_into(T &value)
        {
            volt::deserialize::read_into<T>(this->iter, value);
        }

        template <typename T>
        std::vector<T> *read_new_array()
        {
            return volt::deserialize::read_new_array<T>(this->iter);
        }
    };
} // namespace volt

#endif
