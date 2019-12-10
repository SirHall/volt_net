#pragma once
#ifndef connection_hpp
#define connection_hpp

// #include <netinet/in.h>
// #include <poll.h>
#include <signal.h>

#include "volt/event/global_event.hpp"
#include "volt/messages/msg_reader.hpp"
#include "volt/messages/msg_rec_pool.hpp"
#include "volt/messages/msg_writer.hpp"
#include "volt/volt_defs.hpp"

#include <atomic>
#include <cstdint>
// #include <fcntl.h>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
// #include <sys/socket.h>
// #include <sys/types.h>
#include <thread>
// #include <unistd.h>
#include <vector>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

using boost::asio::ip::tcp;

namespace volt::net
{
    enum ServerConnectStatus
    {
        Connecting,
        Connected,
        Failed
    };

    typedef std::unique_ptr<std::lock_guard<std::recursive_mutex>> aquired_lock;

    class net_con;
    typedef std::unique_ptr<net_con> connection_ptr;

    /**
     * @brief The class that holds all data related to a network connection
     *
     */
    class net_con
    {
        friend class std::unique_ptr<net_con>;

    private:
        tcp::socket                              sock;
        std::atomic_bool                         con_open = false;
        std::shared_ptr<boost::asio::io_context> io;
        boost::asio::io_context::strand          send_strand;
        boost::asio::io_context::strand          recv_strand;
        // std::thread      thr;
        // std::atomic_bool con_open;

#pragma region Addressing

        // const std::unique_ptr<sockaddr> addr;
        // const socklen_t                 len;
        // const int con_fd;

#pragma endregion

#pragma region Send / Recieve Buffers

        std::vector<net_word> buff;
        message_ptr           recv_msg; // The message currently being built
        bool                  next_is_escape =
            false; // Is the next character being read an escape sequence?
        std::mutex            send_buff_mut;
        std::vector<net_word> send_buff;

        std::vector<message_ptr> recieved_messages;

        // message_iter iter;
        std::size_t current_index = 0;
        std::size_t msg_size      = 0;

        std::function<void(con_id)>              con_closed_callback;
        std::function<void(con_id, message_ptr)> new_msg_callback;
        const con_id                             id;
        std::atomic_bool                         is_reading = false;
        // std::mutex _msg_mut;
        // std::mutex _send_buff_mut;

#pragma endregion

#pragma Recieve From Recieve - Buffer

        // void recieve_next_buf();

        bool get_next_byte(net_word &next_byte);

#pragma endregion

#pragma region Message Reception

        // void loop();

        // void recieve_msg();

#pragma endregion

    private:
        // Creates a new net_con given the TCP file descriptor
        net_con(tcp::socket                              connected_socket,
                std::shared_ptr<boost::asio::io_context> io_context,
                con_id                                   connection_id);

        void handle_write(const boost::system::error_code &err,
                          std::size_t                      bytes_transferred);

        void handle_read(const boost::system::error_code &err,
                         std::size_t                      bytes_transferred);

        void schedule_read_handler();

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
        con_id get_con_id() const { return id; }

        // bool connect(const tcp::resolver::results_type &endpoints,
        //              boost::system::error_code &        err);

#pragma region Message Transmission

        /**
         * @brief Sends a message over the connection
         *
         * @param m The message to send
         * @return true The message was sent successfully
         * @return false The message could not be sent
         */
        void send_msg(message_ptr const &m);

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
        static std::unique_ptr<net_con>
            new_connection(tcp::socket connected_socket,
                           std::shared_ptr<boost::asio::io_context> io_context,
                           con_id connection_id);

        void close_con();
        bool is_open();

        void set_closed_callback(std::function<void(con_id)> callback);
        void set_new_msg_callback(
            std::function<void(con_id, message_ptr)> callback);

#pragma endregion
    };
} // namespace volt::net

#endif
