#pragma once
#ifndef serialization_ext_hpp
#define serialization_ext_hpp

#include "volt/net/serialization/net_serialize.hpp"
#include "volt/net/volt_net_defs.hpp"
#include <array>
#include <list>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace volt::net::serialize
{
    // These functions are declared below
    /**
     * @brief Writes a message array size to the message
     *
     * @tparam
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(message_array_size const &v, message_ptr &data);

    /**
     * @brief Writes a net_word (a single byte) to the message
     *
     * @tparam
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(net_word const &v, message_ptr &data);

    /**
     * @brief Writes raw bytes into the array, taking endianness into account
     *
     * @param begin The start of the source byte array
     * @param end The end of the source byte array
     * @param data The message to write the source byte array to
     * @param write_size [deprecated]
     */
    void write_into_byte_array(net_word const *begin, net_word const *end,
                               message_ptr &data, bool correct_endianness,
                               bool write_size);

#pragma region Safe Integers

    /**
     * @brief Writes a single char to the message
     *
     * @tparam  char
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(std::int8_t const &v, message_ptr &data);

    /**
     * @brief Writes a uint8 to the message
     *
     * @tparam  uint8_t
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(std::uint8_t const &v, message_ptr &data);

    /**
     * @brief Writes an int8 to the message
     *
     * @tparam int8_t
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(std::int8_t const &v, message_ptr &data);

    /**
     * @brief Writes a uint16 to the message
     *
     * @tparam uint16_t
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(std::uint16_t const &v, message_ptr &data);

    /**
     * @brief Writes an int16 to the message
     *
     * @tparam  int16_t
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(std::int16_t const &v, message_ptr &data);

    /**
     * @brief Writes a uint32 to the message
     *
     * @tparam  uint32_t
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(std::uint32_t const &v, message_ptr &data);

    /**
     * @brief Writes an int32 to the message
     *
     * @tparam  int32_t
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(std::int32_t const &v, message_ptr &data);

    /**
     * @brief Writes a uint64 to the message
     *
     * @tparam  uint64_t
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(std::uint64_t const &v, message_ptr &data);

    /**
     * @brief Writes an int64 to the message
     *
     * @tparam  int64_t
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(std::int64_t const &v, message_ptr &data);

#pragma endregion

    /**
     * @brief Writes a string to the message
     *
     * @tparam std::string
     * @param v The string to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(std::string const &v, message_ptr &data);

    /**
     * @brief Writes a double to the message
     *
     * @tparam  double
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(double const &v, message_ptr &data);

    // I'm aware that it may be better to implement this more 'generically',
    // though there are many instances where it would simply be better to
    // implement a structure-specific serialization/deserialization function,
    // for example with linked lists. It's better to write only each node's
    // data, instead of the nodes themselves.

    /**
     * @brief Writes a vector to the message
     *
     * @tparam  std::vector<T>
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <typename T>
    void write_into(std::vector<T> const &v, message_ptr &data)
    {
        write_into_array(v, data, true);
    }

    /**
     * @brief Writes an array to the message
     *
     * @tparam  std::array<T, Len>
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <typename T, std::size_t Len>
    void write_into(std::array<T, Len> const &v, message_ptr &data)
    {
        for (auto const &elem : v)
            write_into(elem, data);
    }

    /**
     * @brief Writes a pair to the message
     *
     * @tparam  std::pair<TA, TB>
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <typename TA, typename TB>
    void write_into(std::pair<TA, TB> const &v, message_ptr &data)
    {
        write_into(std::get<0>(v), data);
        write_into(std::get<1>(v), data);
    }

    /**
     * @brief Writes a series of values with differing types to the message
     *
     * @tparam T, Ts...
     * @param v The value to write to the message
     * @param vs All other parameters to be written
     * @param data The message to write to
     */
    template <typename T, typename... Ts>
    void write_into(T const &v, Ts... vs, message_ptr &data)
    {
        write_into(v, data);
        write_into(vs..., data);
    }

    /**
     * @brief Writes a tuple to the message
     *
     * @tparam T, Ts...
     * @param v The value to write to the message
     * @param vs All other parameters to be written
     * @param data The message to write to
     */
    template <typename... Ts>
    void write_into(std::tuple<Ts...> const &v, message_ptr &data)
    {
        // write_into(v..., data);
        std::apply(write_into, v, data);
    }

} // namespace volt::net::serialize

namespace volt::net::deserialize
{

    namespace unsafe
    {
        void *net_to_host_endian(void *ptr, std::size_t size,
                                 void *dest_ptr = nullptr);
        template <typename T>
        void read_into_raw(T &v, void *ptr);
    } // namespace unsafe

#pragma region Safe Integers

    /**
     * @brief Specifically reads an integer from a message into an instance of
     * that integer
     *
     * @tparam T Any integer
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <typename T>
    void read_into_int(message_iter &iterator, T &instance);

    /**
     * @brief Reads a single char from the message
     *
     * @tparam char
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <>
    void read_into(message_iter &iterator, char &instance);

    /**
     * @brief Reads a uint8 from the message
     *
     * @tparam std::uint8_t
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <>
    void read_into(message_iter &iterator, std::uint8_t &instance);

    /**
     * @brief Reads an int8 from the message
     *
     * @tparam std::int8_t
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <>
    void read_into(message_iter &iterator, std::int8_t &instance);

    /**
     * @brief Reads a uint16 from the message
     *
     * @tparam  std::uint16_t
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <>
    void read_into(message_iter &iterator, std::uint16_t &instance);

    /**
     * @brief Reads an int16 from the message
     *
     * @tparam std::int16_t
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <>
    void read_into(message_iter &iterator, std::int16_t &instance);

    /**
     * @brief Reads a uint32 from the message
     *
     * @tparam  std::uint32_t
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <>
    void read_into(message_iter &iterator, std::uint32_t &instance);

    /**
     * @brief Reads an int32 from the message
     *
     * @tparam  std::int32_t
     * @param iterator The message to reade from
     * @param instance The instance to read into
     */
    template <>
    void read_into(message_iter &iterator, std::int32_t &instance);

    /**
     * @brief Reads a uint64 from the message
     *
     * @tparam  std::uint64_t
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <>
    void read_into(message_iter &iterator, std::uint64_t &instance);

    /**
     * @brief Reads an int64 from the message
     *
     * @tparam  std::int64_t
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <>
    void read_into(message_iter &iterator, std::int64_t &instance);

#pragma endregion

    /**
     * @brief Reads a string from the message
     *
     * @tparam  std::string
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <>
    void read_into(message_iter &iterator, std::string &instance);

    /**
     * @brief Reads a vector from the message
     *
     * @tparam  std::vector<T>
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <typename T>
    void read_into(message_iter &iterator, std::vector<T> &instance)
    {
        read_into_array(iterator, instance);
    }

    /**
     * @brief Reads an array from the message
     *
     * @tparam  std::array<T, Len>
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <typename T, std::size_t Len>
    void read_into(message_iter &iterator, std::array<T, Len> &instance)
    {
        for (std::size_t i = 0; i < instance.size(); i++)
            deserialize::read_into(iterator, instance[i]);
    }

    /**
     * @brief Reads a pair from the message
     *
     * @tparam std::pair<TA, TB>
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <typename TA, typename TB>
    void read_into(message_iter &iterator, std::pair<TA, TB> &instance)
    {
        deserialize::read_into<TA>(iterator, std::get<0>(instance));
        deserialize::read_into<TA>(iterator, std::get<1>(instance));
    }

    /**
     * @brief Reads a series of values from the message
     *
     * @tparam  T, Ts...
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <typename T, typename... Ts>
    void read_into(message_iter &iterator, T &instance, Ts... Is)
    {
        deserialize::read_into(iterator, instance);
        deserialize::read_into(iterator, Is...);
    }

    /**
     * @brief Reads a series of values from the message
     *
     * @tparam  Ts...
     * @param iterator The message to read from
     * @param instance The instance to read into
     */
    template <typename... Ts>
    void read_into(message_iter &iterator, Ts... Is)
    {
        deserialize::read_into(iterator, Is...);
    }

} // namespace volt::net::deserialize

#endif
