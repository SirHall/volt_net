#include "volt/listen/listener.hpp"
#include "volt/messages/msg_writer.hpp"
#include "volt/net_con.hpp"
#include "volt/serialization/serialization_ext.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

static std::atomic_bool quit = false;

void handle_close(int s)
{
    quit = true;
    std::cout << "\n\tClosing client (" << s << ")" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Use format is 'client [target ip] [target port]'"
                  << std::endl;
        return -1;
    }

    signal(SIGINT, handle_close); // User pressed Ctrl + C

    std::cout << "Connecting to server..." << std::endl;
    // int con_stat = volt::net_con::server_connect(socket_fd, addr);
    int con_stat = volt::net_con::server_connect(argv[1], argv[2]);
    std::cout << "Connection Status: " << con_stat << std::endl;

    std::string line = "";

    while (!quit && (con_stat == 0) && std::getline(std::cin, line))
    {

        auto writer = volt::msg_writer();
        writer.write_msg<std::string>(line);

        {
            auto lock = volt::net_con::aquire_lock();

            if (auto &con = volt::net_con::get_con(0, lock))
            {
                if (!con->send_msg(writer.get_msg()))
                {
                    quit = true; // If send fails, quit
                    break;
                }
            }
            else
            {
                quit = true;
                break;
            }
        }
    }

    std::cout << "Closing client..." << std::endl;

    {
        auto lock = volt::net_con::aquire_lock();
        auto ids  = volt::net_con::get_con_ids(lock);
        for (unsigned int i = 0; i < volt::net_con::con_count(lock); i++)
            volt::net_con::close_con(ids.at(i), lock);
    }

    std::cout << "Finished thread closing" << std::endl;

    return 0;
}
