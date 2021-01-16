#pragma once
#ifndef VOLT_NET_MESSAGES_MSG_READER_HPP
#define VOLT_NET_MESSAGES_MSG_READER_HPP

#include "volt/net/messages/msg_pool.hpp"
#include "volt/net/serialization/net_serialize.hpp"
#include "volt/net/serialization/serialization_ext.hpp"
#include "volt/net/volt_net_defs.hpp"
#include <memory>

namespace volt::net
{
    /**
     * @brief Reads and helps to transform binary data from a given message back
     * into objects.
     *
     */
    class msg_reader
    {
    private:
        message_ptr const msg;
        message_iter      iter;

    public:
        /**
         * @brief Construct a new msg reader object given a message
         *
         * @param message The pre-existing message for this reader to read from
         */
        msg_reader(message_ptr const message);

        /**
         * @brief Construct a new msg reader without any message
         *
         */
        msg_reader();

        /**
         * @brief Destroy the msg reader object
         *
         */
        ~msg_reader();

        /**
         * @brief Reads any given type from the message given that type's read
         * template function has been defined
         *
         * @tparam T The type to read
         * @param instance The instance to read into
         */
        template <typename T>
        void read_msg(T &instance)
        {
            volt::net::deserialize::read_into(iter, instance);
        }

        /**
         * @brief Specifically reads any integer from the message. You should
         * just use read_msg
         *
         * @tparam T The type of the integer to read
         * @param instance The instance of the integer to read into
         */
        template <typename T>
        void read_msg_int(T &instance)
        {
            volt::net::deserialize::read_into_int(iter, instance);
        }
    };

    typedef std::unique_ptr<msg_reader> reader_ptr;

    /**
     * @brief Instantiates a new instance of a reader without a message
     *
     * @return reader_ptr A smart pointer holding the new reader
     */
    reader_ptr make_reader();

    /**
     * @brief Creates a new instance of a reader given a pointer to a message
     *
     * @param msg The messahe for this reader to read from
     * @return reader_ptr A smart pointer holding the new reader instance
     */
    reader_ptr make_reader(message_ptr const msg);
} // namespace volt::net

#endif