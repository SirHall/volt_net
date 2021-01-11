#include "volt/net/serialization/serialization_ext.hpp"
#include <limits>

using namespace volt::net;

void serialize::write_into_byte_array(net_word const *begin,
                                      net_word const *end, message_ptr &data,
                                      bool correct_endianness, bool write_size)
{
    if (write_size)
        write_into<message_array_size>((message_array_size)(end - begin), data);
    data->reserve(data->size() + (end - begin));
    if (volt::net::is_big_endian() || !correct_endianness)
        for (auto it = begin; it < end; it++)
            write_into<net_word>(*it, data);
    else
        for (auto it = end - 1; it >= begin; it--)
            write_into<net_word>(*it, data);
}

#pragma region Serialize Safe Integers

template <>
void serialize::write_into(char const &v, message_ptr &data)
{
    write_into_byte_array((net_word *)&v, (net_word *)&v + sizeof(char), data,
                          true, false);
}

template <>
void serialize::write_into(net_word const &v, message_ptr &data)
{
    write_byte(v, data);
}

template <>
void serialize::write_into(std::int8_t const &v, message_ptr &data)
{
    write_into_byte_array((net_word *)&v, (net_word *)&v + sizeof(std::int8_t),
                          data, true, false);
}

template <>
void serialize::write_into(std::uint16_t const &v, message_ptr &data)
{
    write_into_byte_array((net_word *)&v,
                          (net_word *)&v + sizeof(std::uint16_t), data, true,
                          false);
}

template <>
void serialize::write_into(std::int16_t const &v, message_ptr &data)
{
    write_into_byte_array((net_word *)&v, (net_word *)&v + sizeof(std::int16_t),
                          data, true, false);
}

template <>
void serialize::write_into(std::uint32_t const &v, message_ptr &data)
{
    write_into_byte_array((net_word *)&v,
                          (net_word *)&v + sizeof(std::uint32_t), data, true,
                          false);
}

template <>
void serialize::write_into(std::int32_t const &v, message_ptr &data)
{
    write_into_byte_array((net_word *)&v, (net_word *)&v + sizeof(std::int32_t),
                          data, true, false);
}

template <>
void serialize::write_into(std::uint64_t const &v, message_ptr &data)
{
    write_into_byte_array((net_word *)&v,
                          (net_word *)&v + sizeof(std::uint64_t), data, true,
                          false);
}

template <>
void serialize::write_into(std::int64_t const &v, message_ptr &data)
{
    write_into_byte_array((net_word *)&v, (net_word *)&v + sizeof(std::int64_t),
                          data, true, false);
}

#pragma endregion

template <>
void serialize::write_into(bool const &v, message_ptr &data)
{
    serialize::write_into(std::uint8_t(v), data);
}

template <>
void serialize::write_into(std::string const &v, message_ptr &data)
{
    // TODO: Really slow
    auto char_vec = std::vector<char>();
    char_vec.reserve(v.size());
    for (char c : v)
        char_vec.push_back(c);
    serialize::write_into_array(char_vec, data, true);
}

template <>
void serialize::write_into(float const &v, message_ptr &data)
{
    if (std::numeric_limits<float>::is_iec559)
    {
        write_into_byte_array((net_word *)&v, ((net_word *)&v) + sizeof(double),
                              data, true, false);
        return;
    }
    // Not IEEE 574 format, build representation manually
}

template <>
void serialize::write_into(double const &v, message_ptr &data)
{
    if (std::numeric_limits<double>::is_iec559)
    {
        write_into_byte_array((net_word *)&v, ((net_word *)&v) + sizeof(double),
                              data, true, false);
        return;
    }
    // Not IEEE 574 format, build representation manually
}

void *deserialize::unsafe::net_to_host_endian(void *ptr, std::size_t size,
                                              void *dest_ptr)
{
    if (is_big_endian())
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
void deserialize::unsafe::read_into_raw(T &v, void *ptr)
{
    v = *((T *)net_to_host_endian(ptr, sizeof(T)));
}

#pragma region Safe Integers

template <typename T>
void deserialize::read_into_int(message_iter &iterator, T &instance)
{
    static_assert(std::is_integral<T>(), "T must be of an integer type");
    deserialize::unsafe::net_to_host_endian(&*iterator, sizeof(T), &instance);
    iterator += sizeof(T);
}

template <>
void deserialize::read_into(message_iter &iterator, char &instance)
{
    read_into_int<char>(iterator, instance);
}

template <>
void deserialize::read_into(message_iter &iterator, std::uint8_t &instance)
{
    read_into_int<std::uint8_t>(iterator, instance);
}

template <>
void deserialize::read_into(message_iter &iterator, std::int8_t &instance)
{
    read_into_int<std::int8_t>(iterator, instance);
}

template <>
void deserialize::read_into(message_iter &iterator, std::uint16_t &instance)
{
    read_into_int<std::uint16_t>(iterator, instance);
}

template <>
void deserialize::read_into(message_iter &iterator, std::int16_t &instance)
{
    read_into_int<std::int16_t>(iterator, instance);
}

template <>
void deserialize::read_into(message_iter &iterator, std::uint32_t &instance)
{
    read_into_int<std::uint32_t>(iterator, instance);
}

template <>
void deserialize::read_into(message_iter &iterator, std::int32_t &instance)
{
    read_into_int<std::int32_t>(iterator, instance);
}

template <>
void deserialize::read_into(message_iter &iterator, std::uint64_t &instance)
{
    read_into_int<std::uint64_t>(iterator, instance);
}

template <>
void deserialize::read_into(message_iter &iterator, std::int64_t &instance)
{
    read_into_int<std::int64_t>(iterator, instance);
}

#pragma endregion

template <>
void deserialize::read_into(message_iter &iterator, bool &instance)
{
    std::uint8_t val;
    deserialize::read_into(iterator, val);
    instance = bool(val);
}

template <>
void deserialize::read_into(message_iter &iterator, std::string &instance)
{
    // TODO: Slow!

    auto char_vec = std::vector<char>();
    deserialize::read_into_array<char>(iterator, char_vec);
    for (char c : char_vec)
        instance += c;
}

// TODO: The way the floats and doubles are deserialized is very unreliable
// across different machines!

template <>
void deserialize::read_into(message_iter &iterator, float &instance)
{
    if (std::numeric_limits<float>::is_iec559)
    {
        deserialize::unsafe::net_to_host_endian(&*iterator, sizeof(float),
                                                &instance);
        iterator += sizeof(float);
    }
    // Not IEEE 574 format, read representation manually
}

template <>
void deserialize::read_into(message_iter &iterator, double &instance)
{
    if (std::numeric_limits<double>::is_iec559)
    {
        deserialize::unsafe::net_to_host_endian(&*iterator, sizeof(double),
                                                &instance);
        iterator += sizeof(double);
    }
    // Not IEEE 574 format, read representation manually
}
