#pragma once
#ifndef message_hpp
#define message_hpp

#include "volt/defs.hpp"
// #include "volt/serialization.hpp"
#include "volt/serialization_ext.hpp"
#include <vector>

namespace volt
{
    class message : public std::vector<net_word>
    {
      private:
        volt::message_iter iter;
        volt::net_tag      tag;

        // void write_buffer_size()
        // {
        //     volt::buffer_size buff_size = this->size() *
        //     sizeof(volt::net_word);
        //     volt::serialize::write_into<volt::buffer_size>(buff_size,
        //                                                    &*this->begin());
        // }

        // void write_tag(volt::net_tag msg_tag)
        // {
        //     volt::serialize::write_into<volt::buffer_size>(
        //         msg_tag, &*this->begin() + sizeof(volt::buffer_size));
        // }

      public:
        // message(volt::net_tag msg_tag) : tag(msg_tag)
        // {
        //     this->write<volt::net_tag>(msg_tag);
        //     this->reset_iterator();
        // }

        message(volt::net_tag msg_tag = 0)
        {
            // this->tag = msg_tag;
            // this->write<volt::buffer_size>(0);
            // this->write<volt::net_tag>(msg_tag);
            // this->reset_iterator();
        }

        ~message() = default;

        // void reset_iterator()
        // {

        //     iter = begin() + sizeof(volt::buffer_size) +
        //     sizeof(volt::net_tag);
        // }

        void *      get_buffer() const { return (void *)&(*this->begin()); }
        std::size_t get_buffer_len() const { return this->size(); }

        // template <typename T>
        // void write(T const &v)
        // {
        //     volt::serialize::write_into<T>(v, *this);
        // }

        // template <typename T>
        // void write_array(std::vector<T> &array)
        // {
        //     volt::serialize::write_into_array<T>(array, *this);
        // }

        // template <typename T>
        // T *read_new()
        // {
        //     return volt::deserialize::read_new<T>(this->iter);
        // }

        // template <typename T>
        // void read_into(T &value)
        // {
        //     volt::deserialize::read_into<T>(this->iter, value);
        // }

        // template <typename T>
        // std::vector<T> *read_new_array()
        // {
        //     return volt::deserialize::read_new_array<T>(this->iter);
        // }
    }; // namespace volt
} // namespace volt

#endif
