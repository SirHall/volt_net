#pragma once
#ifndef serialization_hpp
#define serialization_hpp

#include "volt/defs.hpp"
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace volt::serialize
{

    template <typename T>
    std::vector<net_word> write_new(T const &v)
    {
        auto data = std::vector<net_word>();
        write_into(v, data);
        return data;
    }

    template <typename T>
    void write_into(T const &v, std::vector<net_word> &data)
    {
        // For any type without an explicit specialization, just pretty much
        // copy over the memory data
        constexpr auto array_len = sizeof(T) / sizeof(net_word);

        const auto prev_size = data.size();
        data.resize(data.size() + array_len);
        // No clue how safe this is, but it works
        memcpy(&*(data.begin() + prev_size), &v, sizeof(T));
    }

    template <typename T>
    void write_into_array(std::vector<T> const &v, std::vector<net_word> &data)
    {
        // data.push_back((message_array_size)v.size());
        write_into<message_array_size>((message_array_size)v.size(), data);
        for (T value : v)
            write_into(value, data);
    }
} // namespace volt::serialize

namespace volt::deserialize
{
    template <typename T>
    void read_into(message_iter &iterator, T &instance)
    {
        // If no specialization if made for a class, just memcopy it into
        // a new instance of that class
        memcpy(&instance, &(*iterator), sizeof(T));
        iterator += sizeof(T) / sizeof(net_word);
    }

    template <typename T>
    T *read_new(volt::message_iter &iterator)
    {
        T *instance = new T();
        read_into(iterator, *instance);
        return instance;
    }

    template <typename T>
    void read_into_array(message_iter &iterator, std::vector<T> &array)
    {
        message_array_size array_size;
        read_into<message_array_size>(iterator, array_size);
        array.reserve(array.size() + array_size);

        for (message_array_size i = 0; i < array_size; i++)
        {
            T val;
            read_into<T>(iterator, val);
            array.push_back(val);
        }
    }

    template <typename T>
    std::vector<T> *read_new_array(message_iter &iterator)
    {
        std::vector<T> *array = new std::vector<T>();
        read_into_array(iterator, *array);
        return array;
    }

} // namespace volt::deserialize

#endif
