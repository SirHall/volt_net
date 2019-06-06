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
      public:
        message()  = default;
        ~message() = default;
        template <typename T>
        void write(T const &v)
        {
            write_into(v, this);
        }
    };
} // namespace volt

#endif
