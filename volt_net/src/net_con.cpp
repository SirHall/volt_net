#include "volt/net_con.hpp"
// #include "volt/con_pool.hpp"
#include "volt/addr_resolver.hpp"
#include "volt/event/global_event.hpp"
#include "volt/event_types/e_closed_con.hpp"
#include "volt/event_types/e_new_con.hpp"
#include "volt/volt_defs.hpp"

#include <assert.h>
#include <iostream>
#include <poll.h>
#include <string>

static std::unordered_map<volt::con_id, volt::connection_ptr> cons_map;

static std::recursive_mutex cons_map_mut;

static std::unique_ptr<volt::net_con> null_con =
    std::unique_ptr<volt::net_con>(nullptr);

#pragma region Global Functions

volt::con_id volt::net_con::new_connection(int           con_file_descriptor,
                                           aquired_lock &lock)
{
    // auto new_con = std::make_unique<net_con>(std::move(socket), length,
    // con_file_descriptor);
    auto new_con =
        std::unique_ptr<volt::net_con>(new net_con(con_file_descriptor));
    volt::con_id new_id = new_con->get_con_id();
    cons_map.insert(std::pair(new_con->get_con_id(), std::move(new_con)));
    return new_id;
}

bool volt::net_con::con_exists(con_id id, aquired_lock &lock)
{
    return cons_map.find(id) != cons_map.end();
}

bool volt::net_con::send_msg_to(volt::con_id id, volt::message_ptr &msg,
                                aquired_lock &lock)
{
    // std::lock_guard lock(cons_map_mut);
    return cons_map[id]->send_msg(msg);
}

std::size_t volt::net_con::con_count(aquired_lock &lock)
{
    return cons_map.size();
}

volt::aquired_lock volt::net_con::aquire_lock()
{
    return std::make_unique<std::lock_guard<std::recursive_mutex>>(
        cons_map_mut);
}

std::vector<volt::con_id> volt::net_con::get_con_ids(aquired_lock &lock)
{
    auto ids = std::vector<volt::con_id>();
    for (auto i = cons_map.begin(); i != cons_map.end(); i++)
        ids.push_back(i->first);
    return ids;
}

void volt::net_con::close_con(volt::con_id id, volt::aquired_lock &lock)
{
    auto loc_iter = cons_map.find(id);
    if (loc_iter != cons_map.end()) // This connection id exists
        cons_map.erase(loc_iter);
}

volt::connection_ptr &volt::net_con::get_con(volt::con_id        id,
                                             volt::aquired_lock &lock)
{
    auto loc_iter = cons_map.find(id);
    if (loc_iter != cons_map.end())
    { // This is a valid connection
        return loc_iter->second;
    }
    else
    { // Invalid connection, it does not exist
        return null_con;
    }
}

static volt::con_id next_id = 0;

void volt::net_con::assign_next_id() { id = next_id++; }

#pragma endregion

#pragma region Member Functions

void volt::net_con::recieve_next_buf()
{
    // std::cout << "Recieving next buffer" << std::endl;

    while (true)
    {
        if (!con_open)
            return;

        auto poll_res = poll(fds, 1, timeout);
        if (poll_res == -1)
        {
            close_self();
            // volt::con_pool::con_delete(this->get_con_id());
            return;
        }
        else if (poll_res > 0)
        {
            // std::cout << "Poll found recieved data" << std::endl;
            break;
        }
    }

    auto len = recv(con_fd, buff.data(), volt::max_buffer_size, 0);

    if (len < 0)
    { // An error has occurred
        close_self();
        // volt::con_pool::con_delete(this->get_con_id());
    }
    if (len == 0)
    { // Connection closed
        close_self();
        // volt::con_pool::con_delete(this->get_con_id());
    }
    msg_size      = len;
    current_index = 0;
}

volt::net_word volt::net_con::get_next_byte()
{
    while ((msg_size == 0) || (current_index >= msg_size))
    {
        if (con_open)
            recieve_next_buf();
        else
            return 0; // Not really anything good to put here
    }
    auto val = buff.at(current_index);
    current_index++;
    // std::cout << "Next byte: " << (int)val << std::endl;
    return val;
}

// volt::net_word *volt::net_con::get_next_bytes(std::size_t count)
// {
//     volt::net_word *alloc_mem = (volt::net_word *)malloc(count);
//     for (std::size_t i = 0; i < count; i++)
//         alloc_mem[i] = get_next_byte();
//     return alloc_mem;
// }

volt::net_con::net_con(int con_file_descriptor) : con_fd(con_file_descriptor)
{
    con_open = true;
    assign_next_id();
    buff.resize(volt::max_buffer_size);
    send_buff.resize(volt::max_buffer_size);
    current_index = 0;

    fds->fd     = con_file_descriptor;
    fds->events = 0; // Just for that extra layer of safety
    fds->events = POLLIN;

    auto thr = std::thread([=]() { this->loop(); });
    thr.detach();
    using namespace volt::event;
    global_event<e_new_con>::call_event(e_new_con(this->get_con_id()));
}

volt::net_con::~net_con()
{
    con_open = false;
    // std::cout << "Joining thread" << std::endl;
    // thr.join();
    // std::cout << "Thread joined" << std::endl;
    close(con_fd);
    // volt::con_pool::con_delete(this->get_con_id());
    using namespace volt::event;
    global_event<e_closed_con>::call_event(e_closed_con(this->get_con_id()));
}

bool volt::net_con::server_connect(std::string const address,
                                   std::string const port)
{
    auto addrs_found = volt::net::resolve_address(address, port);

    // Attempt to connect to each address structure found
    for (auto rp = addrs_found.get(); rp != nullptr; rp = rp->ai_next)
    {
        int socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (socket_fd < 0)
            continue;
        int con_result = connect(socket_fd, rp->ai_addr, rp->ai_addrlen);
        if (con_result >= 0)
        {
            {
                auto lock = volt::net_con::aquire_lock();
                volt::net_con::new_connection(socket_fd, lock);
            }
            return true; // We found a working address
        }
        close(socket_fd); // Close the old socket
    }

    return false; // We could not connect
}

void volt::net_con::close_self()
{
    con_open  = false;
    auto lock = volt::net_con::aquire_lock();
    volt::net_con::close_con(this->get_con_id(), lock);
}

void volt::net_con::loop()
{
    while (con_open)
        recieve_msg();
}

void volt::net_con::recieve_msg()
{
    volt::message_ptr msg = msg_pool::get_message();
    msg->resize(0);

    while (con_open)
    {
        net_word b = get_next_byte();
        // current_size++;

        if (b == volt::escape_val && con_open) // This is an escape sequence
        {
            net_word next = get_next_byte();
            // current_size++;
            // This is the original character (escape value)
            if (next == volt::msg_origi_char) {}
            // This is the end of the message
            if (next == volt::msg_end_escaped)
                break;
        }

        msg->push_back(b);
    }

    // TODO Not exactly very thread-safe
    if (con_open)
        volt::net::submit::submit_message(std::move(msg));
}

bool volt::net_con::send_msg(volt::message_ptr const &m)
{
    // We could have messages coming in from many different threads
    std::lock_guard _send_buff_lock(_send_buff_mut);

    send_buff.resize(0);
    for (std::size_t i = 0; i < m->size(); i++)
    {
        send_buff.push_back(m->at(i));
        if (m->at(i) == volt::escape_val)
            send_buff.push_back(volt::msg_origi_char);
    }

    send_buff.push_back(volt::escape_val);
    send_buff.push_back(volt::msg_end_escaped);

    // std::cout << "Sending:";
    // for (std::size_t i = 0; i < send_buff.size(); i++)
    //     std::cout << " " << (unsigned int)send_buff.at(i);
    // std::cout << std::endl;

    return send(con_fd, send_buff.data(), send_buff.size(), 0) > 0;
}

#pragma endregion
