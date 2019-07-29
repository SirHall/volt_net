#pragma once
#ifndef serialization_ext_hpp
#define serialization_ext_hpp

#include "volt/serialization/net_serialize.hpp"
#include "volt/volt_defs.hpp"
#include <string>
#include <type_traits>
#include <vector>

namespace volt::serialize
{
    // template <>
    // void write_into(net_word const &v, volt::message_ptr &data)
    // {
    //     // std::cout << "Write: " << (int)v << std::endl;
    //     data->push_back(v);
    // }

    // These functions are declared below
    template <>
    void write_into(message_array_size const &v, volt::message_ptr &data);
    template <>
    void write_into(net_word const &v, volt::message_ptr &data);

    // Writes raw bytes into the array, taking endianness into account
    void write_into_array(net_word const *begin, net_word const *end,
                          volt::message_ptr &data, bool write_size = true);

#pragma region Safe Integers

    template <>
    void write_into(char const &v, volt::message_ptr &data);

    template <>
    void write_into(std::uint8_t const &v, volt::message_ptr &data);

    template <>
    void write_into(std::int8_t const &v, volt::message_ptr &data);

    template <>
    void write_into(std::uint16_t const &v, volt::message_ptr &data);

    template <>
    void write_into(std::int16_t const &v, volt::message_ptr &data);

    template <>
    void write_into(std::uint32_t const &v, volt::message_ptr &data);

    template <>
    void write_into(std::int32_t const &v, volt::message_ptr &data);

    template <>
    void write_into(std::uint64_t const &v, volt::message_ptr &data);

    template <>
    void write_into(std::int64_t const &v, volt::message_ptr &data);

#pragma endregion

    template <>
    void write_into(std::string const &v, volt::message_ptr &data);

    template <>
    void write_into(double const &v, volt::message_ptr &data);

} // namespace volt::serialize

namespace volt::deserialize
{

    namespace unsafe
    {
        void *net_to_host_endian(void *ptr, std::size_t size,
                                 void *dest_ptr = nullptr);
        template <typename T>
        void read_into_raw(T &v, void *ptr);
    } // namespace unsafe

#pragma region Safe Integers

    template <typename T>
    void read_into_int(message_iter &iterator, T &instance);

    template <>
    void read_into(message_iter &iterator, char &instance);

    template <>
    void read_into(message_iter &iterator, std::uint8_t &instance);

    template <>
    void read_into(message_iter &iterator, std::int8_t &instance);

    template <>
    void read_into(message_iter &iterator, std::uint16_t &instance);

    template <>
    void read_into(message_iter &iterator, std::int16_t &instance);

    template <>
    void read_into(message_iter &iterator, std::uint32_t &instance);

    template <>
    void read_into(message_iter &iterator, std::int32_t &instance);

    template <>
    void read_into(message_iter &iterator, std::uint64_t &instance);

    template <>
    void read_into(message_iter &iterator, std::int64_t &instance);

#pragma endregion

    template <>
    void read_into(message_iter &iterator, std::string &instance);
} // namespace volt::deserialize

#endif
