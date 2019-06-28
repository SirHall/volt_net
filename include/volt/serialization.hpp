#pragma once
#ifndef serialization_hpp
#define serialization_hpp

#include "volt/defs.hpp"
#include "volt/endian.hpp"
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace volt::serialize
{
    template <typename T>
    volt::message_ptr write_new(T const &v)
    {
        auto data = std::make_unique<volt::message_t>();
        write_into(v, data);
        return data;
    }

    // Serialization must be defined for ALL objects
    template <typename T>
    void write_into(T const &v, volt::message_ptr &data);

    template <typename T>
    void write_into_array(std::vector<T> const &v, volt::message_ptr &data,
                          bool write_size = true)
    {
        write_into<volt::buffer_size>(v.size(), data);
        for (T inst : v)
            write_into<T>(inst, data);
    }
} // namespace volt::serialize

namespace volt::deserialize
{
    template <typename T>
    void read_into(message_iter &iterator, T &instance);

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
