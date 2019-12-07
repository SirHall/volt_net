#include "volt/net_con.hpp"
// #include "volt/con_pool.hpp"
#include "volt/event/global_event.hpp"
#include "volt/event_types/e_closed_con.hpp"
#include "volt/event_types/e_new_con.hpp"
#include "volt/net/network.hpp"
#include "volt/volt_defs.hpp"

#include <assert.h>
#include <iostream>
// #include <poll.h>
#include <string>

using namespace volt::net;

// static std::unordered_map<volt::con_id, volt::connection_ptr> cons_map;

// static std::recursive_mutex cons_map_mut;

// static std::unique_ptr<volt::net_con> null_con =
//     std::unique_ptr<volt::net_con>(nullptr);

#pragma region Global Functions

std::unique_ptr<net_con>
    net_con::new_connection(tcp::socket connected_socket,
                            std::shared_ptr<boost::asio::io_context> io_context,
                            con_id connection_id)
{
    return std::unique_ptr<net_con>(
        new net_con(std::move(connected_socket), io_context, connection_id));
}

// bool net_con::con_exists(con_id id, aquired_lock &lock)
// {
//     return cons_map.find(id) != cons_map.end();
// }

// bool volt::net_con::send_msg_to(volt::con_id id, volt::message_ptr &msg,
//                                 aquired_lock &lock)
// {
//     // std::lock_guard lock(cons_map_mut);
//     return cons_map[id]->send_msg(msg);
// }

// std::size_t net_con::con_count(aquired_lock &lock) { return cons_map.size();
// }

// volt::aquired_lock net_con::aquire_lock()
// {
//     return std::make_unique<std::lock_guard<std::recursive_mutex>>(
//         cons_map_mut);
// }

// std::vector<con_id> net_con::get_con_ids(aquired_lock &lock)
// {
//     auto ids = std::vector<con_id>();
//     for (auto i = cons_map.begin(); i != cons_map.end(); i++)
//         ids.push_back(i->first);
//     return ids;
// }

void net_con::close_con()
{
    this->sock.cancel();
    this->sock.close();
    // auto loc_iter = cons_map.find(id);
    // if (loc_iter != cons_map.end()) // This connection id exists
    //     cons_map.erase(loc_iter);
}

// connection_ptr &net_con::get_con(con_id id, aquired_lock &lock)
// {
//     auto loc_iter = cons_map.find(id);
//     if (loc_iter != cons_map.end())
//     { // This is a valid connection
//         return loc_iter->second;
//     }
//     else
//     { // Invalid connection, it does not exist
//         return null_con;
//     }
// }

// static con_id next_id = 0;

// void net_con::assign_next_id() { id = next_id++; }

#pragma endregion

#pragma region Member Functions

// void net_con::recieve_next_buf()
// {
// std::cout << "Recieving next buffer" << std::endl;

// while (true)
// {
//     if (!con_open)
//         return;

//     auto poll_res = poll(fds, 1, timeout);
//     if (poll_res == -1)
//     {
//         close_self();
//         // volt::con_pool::con_delete(this->get_con_id());
//         return;
//     }
//     else if (poll_res > 0)
//     {
//         // std::cout << "Poll found recieved data" << std::endl;
//         break;
//     }
// }

// auto len = recv(con_fd, buff.data(), volt::max_buffer_size, 0);

// if (len < 0)
// { // An error has occurred
//     close_self();
//     // volt::con_pool::con_delete(this->get_con_id());
// }
// if (len == 0)
// { // Connection closed
//     close_self();
//     // volt::con_pool::con_delete(this->get_con_id());
// }
// msg_size      = len;
// current_index = 0;
// }

bool net_con::get_next_byte(net_word &next_byte)
{
    if ((msg_size == 0) || (current_index >= msg_size) || !this->is_open())
    {
        current_index = 0;
        return false;
    }
    next_byte = buff.at(current_index);
    current_index++;
    return true;
}

// volt::net_word *volt::net_con::get_next_bytes(std::size_t count)
// {
//     volt::net_word *alloc_mem = (volt::net_word *)malloc(count);
//     for (std::size_t i = 0; i < count; i++)
//         alloc_mem[i] = get_next_byte();
//     return alloc_mem;
// }

net_con::net_con(tcp::socket                              connected_socket,
                 std::shared_ptr<boost::asio::io_context> io_context,
                 con_id                                   connection_id)
    : sock(std::move(connected_socket)), io(io_context),
      send_strand(*io_context), recv_strand(*io_context), id(connection_id)
{
}

net_con::~net_con()
{
    // std::cout << "Joining thread" << std::endl;
    this->sock.cancel();
    this->sock.close();
    using namespace volt::event;
    global_event<e_closed_con>::call_event(e_closed_con(this->get_con_id()));
}

void net_con::schedule_read_handler()
{
    this->sock.async_read_some(
        boost::asio::buffer(buff),
        boost::asio::bind_executor(
            recv_strand,
            boost::bind(&net_con::handle_read, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)));
}

void net_con::handle_read(const boost::system::error_code &err,
                          std::size_t                      bytes_transferred)
{
    if (!err)
    {
        if (!this->recv_msg)
        {
            this->recv_msg = msg_pool::get_message();
            this->recv_msg->resize(0);
        }

        net_word next_byte = 0;

        while (this->is_open() && get_next_byte(next_byte))
        {
            if ((next_byte != escape_val) && !this->next_is_escape)
            {
                // This is a normal byte
                this->recv_msg->push_back(next_byte);
            }
            else if ((next_byte == escape_val) && !this->next_is_escape)
            {
                // This byte signals that this next character is an escape
                // sequence
                this->next_is_escape = true;
                continue;
            }
            else if (this->next_is_escape)
            {
                // This is the next character in the escape sequence
                switch (next_byte)
                {
                    case msg_origi_char:
                        // This is just the original character
                        this->recv_msg->push_back(escape_val);
                        break;
                    case msg_end_escaped:
                        // This is the end of the message
                        this->new_msg_callback(this->get_con_id(),
                                               std::move(recv_msg));
                        this->recv_msg = msg_pool::get_message();
                        this->recv_msg->resize(0);
                        break;
                    default:
                        // TODO: Throw some 'unrecognized escape sequence' error
                        std::cerr << "Received unrecognized escape sequence: "
                                  << next_byte << std::endl;
                        break;
                }
                // We finished the escape sequence
                this->next_is_escape = false;
            }
        }

        if (this->is_open())
        {
            this->schedule_read_handler();
        }
    }
}

void net_con::send_msg(message_ptr m)
{
    // TODO: Ensure that the message does not get modified or get deleted during
    // this async call
    boost::asio::async_write(
        sock, boost::asio::buffer(*m),
        boost::asio::bind_executor(
            send_strand,
            boost::bind(&net_con::handle_write, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)));
}

void net_con::handle_write(const boost::system::error_code &err,
                           std::size_t                      bytes_transferred)
{
    if (!err) {}
}

bool net_con::is_open() { return this->sock.is_open(); }

void net_con::set_closed_callback(std::function<void(con_id)> callback)
{
    this->con_closed_callback = callback;
}

void net_con::set_new_msg_callback(
    std::function<void(con_id, message_ptr)> callback)
{
    this->new_msg_callback = callback;
}

#pragma endregion
