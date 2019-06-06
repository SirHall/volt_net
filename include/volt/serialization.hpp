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
        1 return data;
    }

    template <typename T>
    void write_into(T const &v, std::vector<net_word> &data)
    {
        // For any type without an explicit specialization, just pretty much
        // copy over the memory data
        constexpr auto array_len = sizeof(T) / sizeof(net_word);

        const prev_size = data.size();
        data.resize(data.size() + array_len);
        // No clue how safe this is, but it works
        memcpy(data.begin() + prev_size, v, sizeof(T));
    }

    template <typename T>
    void write_into_array(std::vector<T> const &v, std::vector<net_word> &data)
    {
        data.push_back((message_array_size)v.size());
        for (T value : v)
            write_into(value, data);
    }

} // namespace volt::serialize

namespace volt::deserialize
{
    template <typename T>
    T *read_new(message_iter &iterator)
    {
        T *instance = new T();
        read_into(iterator, *instance);
        return instance;
    }

    template <typename T>
    void read_into(message_iter &iterator, T &instance)
    {
        // If no specialization if made for a class, just memcopy it into
        // a new instance of that class
        memcpy(&instance, iterator, sizeof(T));

        message_iter += sizeof(T) / sizeof(net_word);
    }

    template <typename T>
    std::vector<T> *read_new_array(message_iter &iterator)
    {
        std::vector<T> *array = new std::vector<T>();
        read_into_array(iterator, *array);
        return array;
    }

    template <typename T>
    void read_into_array(message_iter &iterator, std::vector<T> &array)
    {
        message_array_size array_size = *read_new<message_array_size>(iterator);

        constexpr auto array_len =
            sizeof(message_array_size) / sizeof(net_word);

        message_iter += len;
        array.reserve(array.size() + len);

        for (message_array_size i = 0; i < array_size; i++)
            array.push_back(read_new<T>(*iterator));
    }

} // namespace volt::deserialize

#endif
