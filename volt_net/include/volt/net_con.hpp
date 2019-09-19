#pragma once
#ifndef connection_hpp
#define connection_hpp

#include <netinet/in.h>
#include <poll.h>
#include <signal.h>

#include "volt/event/global_event.hpp"
#include "volt/messages/msg_reader.hpp"
#include "volt/messages/msg_rec_pool.hpp"
#include "volt/messages/msg_writer.hpp"
#include "volt/volt_defs.hpp"
#include <atomic>
#include <cstdint>
#include <fcntl.h>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

namespace volt
{

    typedef std::unique_ptr<std::lock_guard<std::recursive_mutex>> aquired_lock;

    class net_con;
    typedef std::unique_ptr<volt::net_con> connection_ptr;

    /**
     * @brief The class that holds all data related to a network connection
     *
     */
    class net_con
    {
        friend class std::unique_ptr<net_con>;

      private:
        // std::thread      thr;
        std::atomic_bool con_open;

#pragma region Addressing

        // const std::unique_ptr<sockaddr> addr;
        // const socklen_t                 len;
        const int con_fd;

#pragma endregion

#pragma region Send / Recieve Buffers

        std::vector<volt::net_word> buff;
        std::vector<volt::net_word> send_buff;

        std::vector<volt::message_ptr> recieved_messages;

        // volt::message_iter iter;
        std::size_t current_index = 0;
        std::size_t msg_size      = 0;

        std::mutex _msg_mut;
        std::mutex _send_buff_mut;

#pragma endregion

#pragma Recieve From Recieve - Buffer

        void recieve_next_buf();

        volt::net_word get_next_byte();

#pragma endregion

#pragma region Polling

        struct pollfd fds[1];
        int           timeout = 250;

#pragma endregion

#pragma region ID

        volt::con_id id;

        void assign_next_id();

#pragma endregion

#pragma region Message Reception

        void loop();

        void recieve_msg();

#pragma endregion

      private:
        // Creates a new net_congiven the TCP file descriptor
        net_con(int con_file_descriptor);

        void close_self();

      public:
        // No copying is allowed
        /**
         * @brief Forbids network connection object copying
         *
         * @param other The other instance of net_con
         */
        net_con(net_con const &other) = delete;
        /**
         * @brief Destroy the net con object
         *
         */
        ~net_con();

        /**
         * @brief Gets this connection's connection id
         *
         * @return volt::con_id This connection's connection id
         */
        volt::con_id get_con_id() const { return id; }

        /**
         * @brief Attempts to connect to a listening server
         *
         * @param address The target address to connect to
         * @param port The target port to connect through
         * @return bool Returns whether or not a connection was established
         */
        static bool server_connect(std::string const address,
                                   std::string const port);

#pragma region Message Transmission

        /**
         * @brief Sends a message over the connection
         *
         * @param m The message to send
         * @return true The message was sent successfully
         * @return false The message could not be sent
         */
        bool send_msg(volt::message_ptr const &m);

#pragma endregion

#pragma region Global Functions

        /**
         * @brief Creates a new connection instance
         *
         * @param con_file_descriptor The file descriptor the connection data is
         * sent/recieved on
         * @param lock The lock giving us permission to create a new connection
         * @return volt::con_id The connection id of this new connection
         */
        static volt::con_id new_connection(int           con_file_descriptor,
                                           aquired_lock &lock);

        /**
         * @brief Checks for the existance of a connection
         *
         * @param id The connection id of the connection to be checed
         * @param lock The lock allowing us to perform this check
         * @return true The connection does exist
         * @return false The connection does not exist
         */
        static bool con_exists(con_id id, aquired_lock &lock);

        /**
         * @brief Sends a message over any connection
         *
         * @param id The id to send the message over
         * @param msg The message to send over the connection
         * @param lock The lock allowing us to access the connection object
         * @return true The connection was sent successfully
         * @return false THe message could not be sent
         */
        static bool send_msg_to(volt::con_id id, volt::message_ptr &msg,
                                aquired_lock &lock);

        /**
         * @brief Finds the total number of connections
         *
         * @param lock The lock allowing us to access this data
         * @return std::size_t The number of active connections
         */
        static std::size_t con_count(aquired_lock &lock);

        /**
         * @brief Aquires the lock required to perform many static net_con calls
         *
         * @return aquired_lock The lock that gives permission to perform the
         * calls
         */
        static aquired_lock aquire_lock();

        /**
         * @brief Get a list of all active connection's, connection id's
         *
         * @param lock The lock giving us permission to access the connection id
         * list
         * @return std::vector<volt::con_id> The list of all connection id's
         */
        static std::vector<volt::con_id> get_con_ids(aquired_lock &lock);

        /**
         * @brief Closes any specified connection
         *
         * @param id The id of the connection to close
         * @param lock The lock giving us permission to close this connection
         */
        static void close_con(con_id id, aquired_lock &lock);

        /**
         * @brief Get a connection object by its connection id
         *
         * @param id The connection id to search for
         * @param lock The lock that gives us permission to access this object
         * @return connection_ptr& The connection searched for
         */
        static connection_ptr &get_con(con_id id, aquired_lock &lock);

#pragma endregion
    };
} // namespace volt

#endif
