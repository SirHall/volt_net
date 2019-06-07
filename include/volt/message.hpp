#pragma once
#ifndef message_hpp
#define message_hpp

#include "volt/defs.hpp"
#include "volt/serialization.hpp"
#include <vector>

namespace volt
{
    class message : public std::vector<net_word>
    {
      private:
        volt::message_iter iter;

      public:
        message() { reset_iterator(); }
        ~message() = default;

        void reset_iterator() { iter = begin(); }

        template <typename T>
        void write(T const &v)
        {
            volt::serialize::write_into<T>(v, *this);
        }

        template <typename T>
        void write_array(std::vector<T> &array)
        {
            volt::serialize::write_into_array(array, *this);
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
