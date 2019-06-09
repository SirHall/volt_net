#pragma once
#ifndef _hpp
#define _hpp

#include "volt/defs.hpp"
#include "volt/serialization.hpp"
#include <string>
#include <vector>

namespace volt::serialize
{
    template <>
    void write_into(std::string const &v, std::vector<net_word> &data)
    {
        // TODO: Really slow
        auto char_vec = std::vector<char>(v.size());
        for (size_t i = 0; i < v.size(); i++)
            char_vec.at(i) = v.at(i);
        volt::serialize::write_into_array(char_vec, data);
    }

} // namespace volt::serialize

namespace volt::deserialize
{
    template <>
    void read_into(message_iter &iterator, std::string &instance)
    {
        // TODO: Slow!

        auto char_vec = std::vector<char>();
        volt::deserialize::read_into_array<char>(iterator, char_vec);
        for (char c : char_vec)
            instance += c;
    }
} // namespace volt::deserialize

#endif
