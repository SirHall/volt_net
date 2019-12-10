#pragma once
#ifndef net_serialize_hpp
#define net_serialize_hpp

#include "volt/volt_defs.hpp"
#include "volt/volt_endian.hpp"
// #include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace volt::net::serialize
{
    template <typename T>
    message_ptr write_new(T const &v)
    {
        auto data = make_message();
        write_into(v, data);
        return data;
    }

    // Serialization must be defined for ALL objects
    template <typename T>
    void write_into(T const &v, message_ptr &data);
    //     {
    // #error Cannot use default 'write_into', create a template specialization
    // of 'write_into' to use with a custom class.
    //     }

    template <typename T>
    void write_into_array(std::vector<T> const &v, message_ptr &data,
                          bool write_size = true)
    {
        if (write_size)
            write_into<message_array_size>((message_array_size)v.size(), data);
        for (T inst : v)
            write_into<T>(inst, data);
    }
} // namespace volt::net::serialize

namespace volt::net::deserialize
{
    template <typename T>
    void read_into(message_iter &iterator, T &instance);
    //     {
    // #error Cannot use default 'read_into', create a template specialization
    // of 'read_into' to use with a custom class.
    //     }

    template <typename T>
    T *read_new(message_iter &iterator)
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
            T val = T();
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

} // namespace volt::net::deserialize

#endif
