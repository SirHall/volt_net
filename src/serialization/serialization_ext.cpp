#include "volt/serialization/serialization_ext.hpp"

void volt::serialize::write_into_array(net_word const *   begin,
                                       net_word const *   end,
                                       volt::message_ptr &data, bool write_size)
{
    if (write_size)
        write_into<message_array_size>((message_array_size)(end - begin), data);
    data->reserve(data->size() + (end - begin));
    if (volt::net::is_big_endian())
        for (auto it = begin; it < end; it++)
            write_into<net_word>(*it, data);
    else
        for (auto it = end - 1; it >= begin; it--)
            write_into<net_word>(*it, data);
}

#pragma region Serialize Safe Integers

template <>
void volt::serialize::write_into(char const &v, volt::message_ptr &data)
{
    write_into_array((net_word *)&v, (net_word *)&v + sizeof(char), data,
                     false);
}

template <>
void volt::serialize::write_into(std::uint8_t const &v, volt::message_ptr &data)
{
    data->push_back(v);
}

template <>
void volt::serialize::write_into(std::int8_t const &v, volt::message_ptr &data)
{
    write_into_array((net_word *)&v, (net_word *)&v + sizeof(std::int8_t), data,
                     false);
}

template <>
void volt::serialize::write_into(std::uint16_t const &v,
                                 volt::message_ptr &  data)
{
    write_into_array((net_word *)&v, (net_word *)&v + sizeof(std::uint16_t),
                     data, false);
}

template <>
void volt::serialize::write_into(std::int16_t const &v, volt::message_ptr &data)
{
    write_into_array((net_word *)&v, (net_word *)&v + sizeof(std::int16_t),
                     data, false);
}

template <>
void volt::serialize::write_into(std::uint32_t const &v,
                                 volt::message_ptr &  data)
{
    write_into_array((net_word *)&v, (net_word *)&v + sizeof(std::uint32_t),
                     data, false);
}

template <>
void volt::serialize::write_into(std::int32_t const &v, volt::message_ptr &data)
{
    write_into_array((net_word *)&v, (net_word *)&v + sizeof(std::int32_t),
                     data, false);
}

template <>
void volt::serialize::write_into(std::uint64_t const &v,
                                 volt::message_ptr &  data)
{
    write_into_array((net_word *)&v, (net_word *)&v + sizeof(std::uint64_t),
                     data, false);
}

template <>
void volt::serialize::write_into(std::int64_t const &v, volt::message_ptr &data)
{
    write_into_array((net_word *)&v, (net_word *)&v + sizeof(std::int64_t),
                     data, false);
}

#pragma endregion

template <>
void volt::serialize::write_into(std::string const &v, volt::message_ptr &data)
{
    // TODO: Really slow
    auto char_vec = std::vector<char>();
    char_vec.reserve(v.size());
    for (char c : v)
        char_vec.push_back(c);
    volt::serialize::write_into_array(char_vec, data, true);
}

template <>
void volt::serialize::write_into(double const &v, volt::message_ptr &data)
{
    if (std::numeric_limits<double>::is_iec559)
    {
        write_into_array((net_word *)&v, ((net_word *)&v) + sizeof(double),
                         data, false);
        return;
    }
    // Not IEEE 574 format, build representation manually
}

void *volt::deserialize::unsafe::net_to_host_endian(void *ptr, std::size_t size,
                                                    void *dest_ptr)
{
    if (volt::net::is_big_endian())
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
void volt::deserialize::unsafe::read_into_raw(T &v, void *ptr)
{
    v = *((T *)net_to_host_endian(ptr, sizeof(T)));
}

#pragma region Safe Integers

template <typename T>
void volt::deserialize::read_into_int(message_iter &iterator, T &instance)
{
    static_assert(std::is_integral<T>(), "T must be of an integer type");
    volt::deserialize::unsafe::net_to_host_endian(&*iterator, sizeof(T),
                                                  &instance);
    iterator += sizeof(T);
}

template <>
void volt::deserialize::read_into(message_iter &iterator, char &instance)
{
    read_into_int(iterator, instance);
}

template <>
void volt::deserialize::read_into(message_iter &iterator,
                                  std::uint8_t &instance)
{
    read_into_int(iterator, instance);
}

template <>
void volt::deserialize::read_into(message_iter &iterator, std::int8_t &instance)
{
    read_into_int(iterator, instance);
}

template <>
void volt::deserialize::read_into(message_iter & iterator,
                                  std::uint16_t &instance)
{
    read_into_int(iterator, instance);
}

template <>
void volt::deserialize::read_into(message_iter &iterator,
                                  std::int16_t &instance)
{
    read_into_int(iterator, instance);
}

template <>
void volt::deserialize::read_into(message_iter & iterator,
                                  std::uint32_t &instance)
{
    read_into_int(iterator, instance);
}

template <>
void volt::deserialize::read_into(message_iter &iterator,
                                  std::int32_t &instance)
{
    read_into_int(iterator, instance);
}

template <>
void volt::deserialize::read_into(message_iter & iterator,
                                  std::uint64_t &instance)
{
    read_into_int(iterator, instance);
}

template <>
void volt::deserialize::read_into(message_iter &iterator,
                                  std::int64_t &instance)
{
    read_into_int(iterator, instance);
}

#pragma endregion

template <>
void volt::deserialize::read_into(message_iter &iterator, std::string &instance)
{
    // TODO: Slow!

    auto char_vec = std::vector<char>();
    volt::deserialize::read_into_array<char>(iterator, char_vec);
    for (char c : char_vec)
        instance += c;
}