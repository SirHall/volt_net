#pragma once
#ifndef _hpp
#define _hpp

#include "volt/defs.hpp"
#include "volt/serialization.hpp"
#include <string>
#include <type_traits>
#include <vector>

namespace volt::serialize
{
    template <>
    void write_into(net_word const &v, volt::message_ptr &data)
    {
        std::cout << "Write: " << (int)v << std::endl;
        data->push_back(v);
    }

    // This function is declared below
    template <>
    void write_into(message_array_size const &v, volt::message_ptr &data);

    // Writes raw bytes into the array, taking endianness into account
    void write_into_array(net_word const *begin, net_word const *end,
                          volt::message_ptr &data, bool write_size = true)
    {
        if (write_size)
            write_into<message_array_size>((message_array_size)(end - begin),
                                           data);
        // auto prev_size = data->size();
        data->reserve(data->size() + (end - begin));
        if (volt::net::is_be)
            for (auto it = begin; it < end; it++)
                write_into<net_word>(*it, data);
        else
            for (auto it = end - 1; it >= begin; it--)
                write_into<net_word>(*it, data);
    }

#pragma region Primitive Integers

    //     template <>
    //     void write_into(unsigned char const &v, volt::message_ptr &data)
    //     {
    //         write_into<net_word>((std::uint8_t)v, data);
    //     }

    //     template <>
    //     void write_into(signed char const &v, volt::message_ptr &data)
    //     {
    //         write_into<net_word>(*((std::uint8_t *)&v), data);
    //     }

    //     template <>
    //     void write_into(unsigned short const &v, volt::message_ptr &data)
    //     {
    //         std::uint16_t n = v;
    //         write_into_array((net_word *)&n,
    //                          (net_word *)(&n + sizeof(std::uint16_t)), data,
    //                          false);
    //     }

    //     template <>
    //     void write_into(signed short const &v, volt::message_ptr &data)
    //     {
    //         std::int16_t n = v;
    //         write_into_array((net_word *)&n,
    //                          (net_word *)(&n + sizeof(std::int16_t)), data,
    //                          false);
    //     }

    //     template <>
    //     void write_into(unsigned int const &v, volt::message_ptr &data)
    //     {
    //         std::uint32_t n = v;
    //         write_into_array((net_word *)&n,
    //                          (net_word *)(&n + sizeof(std::uint32_t)), data,
    //                          false);
    //     }

    //     template <>
    //     void write_into(signed int const &v, volt::message_ptr &data)
    //     {
    //         std::int32_t n = v;
    //         write_into_array((net_word *)&n,
    //                          (net_word *)(&n + sizeof(std::int32_t)), data,
    //                          false);
    //     }

    //     template <>
    //     void write_into(unsigned long const &v, volt::message_ptr &data)
    //     {
    //         std::uint64_t n = v;
    //         write_into_array((net_word *)&n,
    //                          (net_word *)(&n + sizeof(std::uint64_t)), data,
    //                          false);
    //     }

    //     template <>
    //     void write_into(signed long const &v, volt::message_ptr &data)
    //     {
    //         std::int64_t n = v;
    //         write_into_array((net_word *)&n,
    //                          (net_word *)(&n + sizeof(std::int64_t)), data,
    //                          false);
    //     }

#pragma endregion

#pragma region Safe Integers

    template <>
    void write_into(std::uint16_t const &v, volt::message_ptr &data)
    {
        write_into_array((net_word *)&v, (net_word *)&v + sizeof(std::uint16_t),
                         data, false);
    }

    template <>
    void write_into(std::int16_t const &v, volt::message_ptr &data)
    {
        write_into_array((net_word *)&v, (net_word *)&v + sizeof(std::int16_t),
                         data, false);
    }

    template <>
    void write_into(std::uint32_t const &v, volt::message_ptr &data)
    {
        write_into_array((net_word *)&v, (net_word *)&v + sizeof(std::uint32_t),
                         data, false);
    }

    template <>
    void write_into(std::int32_t const &v, volt::message_ptr &data)
    {
        write_into_array((net_word *)&v, (net_word *)&v + sizeof(std::int32_t),
                         data, false);
    }

    template <>
    void write_into(std::uint64_t const &v, volt::message_ptr &data)
    {
        write_into_array((net_word *)&v, (net_word *)&v + sizeof(std::uint64_t),
                         data, false);
    }

    template <>
    void write_into(std::int64_t const &v, volt::message_ptr &data)
    {
        write_into_array((net_word *)&v, (net_word *)&v + sizeof(std::int64_t),
                         data, false);
    }

#pragma endregion

    template <>
    void write_into(std::string const &v, volt::message_ptr &data)
    {
        // TODO: Really slow
        auto char_vec = std::vector<char>(v.size());
        for (size_t i = 0; i < v.size(); i++)
            char_vec.at(i) = v.at(i);
        volt::serialize::write_into_array(char_vec, data, true);
    }

    template <>
    void write_into(double const &v, volt::message_ptr &data)
    {
        if (std::numeric_limits<double>::is_iec559)
        {
            write_into_array((net_word *)&v, ((net_word *)&v) + sizeof(double),
                             data, false);
            return;
        }
        // Not IEEE 574 format, build representation manually
    }

} // namespace volt::serialize

namespace volt::deserialize
{

    namespace unsafe
    {
        void *net_to_host_endian(void *ptr, std::size_t size,
                                 void *dest_ptr = nullptr)
        {
            if (volt::net::is_be)
                return ptr;

            // Host is little endian, write in backwards
            char *char_dest_ptr;
            if (dest_ptr == nullptr)
                char_dest_ptr = (char *)std::malloc(size);
            else
                char_dest_ptr = (char *)dest_ptr;
            for (std::size_t i = 0; i < size; i++)
                char_dest_ptr[i] = ((char *)ptr)[(size - 1) - i];
            return ptr;
        }

        template <typename T>
        void read_into_raw(T &v, void *ptr)
        {
            v = *((T *)net_to_host_endian(ptr, sizeof(T)));
        }
    } // namespace unsafe

    bool read_is_end(message_iter &iterator)
    {
        if ((*iterator == escape_val) && (*(iterator + 1) == msg_end_escaped))
        {
            iterator += 2;
            return true;
        }
        return false;
    }

    template <>
    void read_into(message_iter &iterator, net_word &instance)
    {
        instance = *iterator;
        if (instance == escape_val)
        {
            iterator++;
            instance = *iterator;
        }
        iterator++;
    }

    template <>
    void read_into(message_iter &iterator, std::string &instance)
    {
        // TODO: Slow!

        auto char_vec = std::vector<char>();
        volt::deserialize::read_into_array<char>(iterator, char_vec);
        for (char c : char_vec)
            instance += c;
    }

    // template <>
    // void read_into(message_iter &iterator, volt::net_word &instance)
    // {
    //     instance = *iterator;
    //     iterator++;
    // }

#pragma region Safe Integers

    template <typename T>
    void read_into_int(message_iter &iterator, T &instance)
    {
        static_assert(std::is_integral<T>(), "T must be of an integer type");
        volt::deserialize::unsafe::net_to_host_endian(&*iterator, sizeof(T),
                                                      &instance);
        iterator += sizeof(T);
    }

#pragma endregion

} // namespace volt::deserialize

#endif
