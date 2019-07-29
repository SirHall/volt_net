#include <iostream>

#include "volt/listen/listener.hpp"
#include "volt/messages/msg_writer.hpp"
#include "volt/net_con.hpp"
#include "volt/serialization/serialization_ext.hpp"

#include <atomic>
#include <chrono>
#include <execinfo.h>
#include <string>
#include <thread>

static std::atomic_bool quit = false;

void handle_close(int s)
{
    std::cout << "\n\tClosing client (" << s << ")" << std::endl;
    quit = true;
}

// Borrowed kindly from https://stackoverflow.com/a/77336
void handle_segfault(int sig)
{
    void * array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Segfault error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

int main(int argc, char *argv[])
{

    signal(SIGINT, handle_close);     // User pressed Ctrl + C
    signal(SIGSEGV, handle_segfault); // Segmentation fault

    std::cout << "Connecting to server..." << std::endl;
    // int con_stat = volt::net_con::server_connect(socket_fd, addr);
    int con_stat = volt::net_con::server_connect();
    std::cout << "Connection Status: " << con_stat << std::endl;

    while (!quit && con_stat == 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        auto        writer = volt::msg_writer();
        std::string text   = "Hello networked world!";
        writer.write_msg<std::string>(text);
        {
            auto lock = volt::net_con::aquire_lock();
            if(!volt::net_con::con_exists(0, lock))
            {
                quit = true;
                break;
            }
            volt::net_con::send_msg_to(0, writer.get_msg(), lock);
        }
        std::cout << "Sent message" << std::endl;
    }

    std::cout << "Closing client..." << std::endl;

    {
        auto lock = volt::net_con::aquire_lock();
        auto ids = volt::net_con::get_con_ids(lock);
        for(unsigned int i = 0; i < volt::net_con::con_count(lock); i++)
            volt::net_con::close_con(ids.at(i), lock);
    }

    std::cout << "Finished thread closing" << std::endl;

    return 0;
}
