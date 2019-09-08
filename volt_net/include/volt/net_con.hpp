#pragma once
#ifndef connection_hpp
#define connection_hpp

#include <netinet/in.h>
#include <poll.h>
#include <signal.h>

#include "volt/global_events/global_event.hpp"
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
        net_con(net_con const &other) = delete;
        ~net_con();

        volt::con_id get_con_id() const { return id; }

        // Attempts to connect to a listening server, returns state
        static int server_connect(std::string const address,
                                  std::string const port);

#pragma region Message Transmission

        bool send_msg(volt::message_ptr const &m);

#pragma endregion

#pragma region GLobal Functions

        static volt::con_id new_connection(int           con_file_descriptor,
                                           aquired_lock &lock);

        static bool con_exists(con_id id, aquired_lock &lock);

        static bool send_msg_to(volt::con_id id, volt::message_ptr &msg,
                                aquired_lock &lock);

        static std::size_t con_count(aquired_lock &lock);

        static aquired_lock aquire_lock();

        static std::vector<volt::con_id> get_con_ids(aquired_lock &lock);

        static void close_con(con_id id, aquired_lock &lock);

        static connection_ptr &get_con(con_id id, aquired_lock &lock);

#pragma endregion
    };
} // namespace volt

#endif
