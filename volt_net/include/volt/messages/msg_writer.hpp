#pragma once
#ifndef msg_writer_hpp
#define msg_writer_hpp

#include "volt/messages/msg_pool.hpp"
#include "volt/serialization/net_serialize.hpp"
#include <memory>

namespace volt
{
    /**
     * @brief Prepares a message with data before it is sent over a connection.
     *
     */
    class msg_writer
    {
      private:
        volt::message_ptr msg;

      public:
        /**
         * @brief Construct a new msg writer object given a message
         *
         * @param message A pre-existing message instance
         */
        msg_writer(volt::message_ptr message);

        /**
         * @brief Construct a new msg writer object and retrieves a message
         * instance from msg_pool
         *
         */
        msg_writer();

        /**
         * @brief Destroy the msg writer object
         *
         */
        ~msg_writer();

        /**
         * @brief Writes any type to the message given its write-template
         * function is defined
         *
         * @tparam T The type to write into the message
         * @param instance The instance of the type to write into the message
         */
        template <typename T>
        void write_msg(T const &instance)
        {
            volt::serialize::write_into<T>(instance, msg);
        }

        /**
         * @brief Get the message object
         *
         * @return volt::message_ptr& The message instance that this writer is
         * holding
         */
        volt::message_ptr &get_msg();
    };

    typedef std::unique_ptr<msg_writer> writer_ptr;

    /**
     * @brief Instantiates a new instance of a writer
     *
     * @return writer_ptr The new writer
     */
    writer_ptr make_writer();
    /**
     * @brief Instantiates a new instance of a writer with a given message
     *
     * @param msg The message for this new writer to write to
     * @return writer_ptr The new writer
     */
    writer_ptr make_writer(message_ptr msg);

} // namespace volt

#endif
