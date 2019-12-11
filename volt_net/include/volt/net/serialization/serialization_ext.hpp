#pragma once
#ifndef serialization_ext_hpp
#define serialization_ext_hpp

#include "volt/net/serialization/net_serialize.hpp"
#include "volt/net/volt_net_defs.hpp"
#include <string>
#include <type_traits>
#include <vector>

namespace volt::net::serialize
{
    // template <>
    // void write_into(net_word const &v, volt::message_ptr &data)
    // {
    //     // std::cout << "Write: " << (int)v << std::endl;
    //     data->push_back(v);
    // }

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
    void write_into_array(net_word const *begin, net_word const *end,
                          message_ptr &data, bool write_size = true);

#pragma region Safe Integers

    /**
     * @brief Writes a single char to the message
     *
     * @tparam  char
     * @param v The value to write to the message
     * @param data The message to write to
     */
    template <>
    void write_into(char const &v, message_ptr &data);

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
} // namespace volt::net::deserialize

#endif
