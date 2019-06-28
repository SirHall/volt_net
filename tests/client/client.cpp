#include <iostream>

#include "volt/connection.hpp"
#include "volt/listen/listener.hpp"
#include "volt/message.hpp"
#include "volt/messages/msg_writer.hpp"
#include "volt/serialization_ext.hpp"

#include <chrono>
#include <string>
#include <thread>

int main(int argc, char *argv[])
{

    sockaddr_in addr;
    int         socket_fd = 0;

    int con_stat = volt::connection::server_connect(socket_fd, addr);

    std::cout << "Connection Status: " << con_stat << std::endl;

    auto con = volt::connection();

    // sockaddr *socket, socklen_t length,
    //                  int con_file_descriptor

    con.add_protocol(std::make_unique<volt::protocol::tcp_protocol>(
        (sockaddr *)&addr, sizeof(addr), socket_fd));

    auto writer = volt::msg_writer();

    writer.write_msg<std::int64_t>(-255);

    auto &msg = writer.get_msg();
    con.send_message(msg, 0);

    std::cout << "Sent message" << std::endl;

    return 0;
}
