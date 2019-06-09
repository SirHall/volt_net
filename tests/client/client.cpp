#include <iostream>

#include "volt/connection.hpp"
#include "volt/listen/listener.hpp"
#include "volt/message.hpp"
#include "volt/serialization.hpp"
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

    auto m = volt::message();

    m.write<std::string>("Hello world");

    con.send_message(m, 0);

    std::cout << "Sent message" << std::endl;

    return 0;
}
