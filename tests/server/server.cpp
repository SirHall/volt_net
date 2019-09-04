#include <iostream>

#include "volt/global_events/observer.hpp"
#include "volt/listen/listener.hpp"
#include "volt/messages/msg_reader.hpp"
#include "volt/messages/msg_rec_pool.hpp"
#include "volt/net_con.hpp"
#include "volt/serialization/serialization_ext.hpp"

#include <atomic>
#include <chrono>
#include <signal.h>
#include <string>
#include <thread>

struct test
{
    int    a = 1;
    double b = 3.141592658;
    char   c = 'c';
};

static std::atomic_bool quit = false;

void handle_close(int s)
{
    std::cout << "\n\tClosing server (" << s << ")" << std::endl;
    quit = true;
}

void msg_event(volt::reader_ptr const &reader)
{
    std::string str = "";
    reader->read_msg(str);
    std::cout << "\t\t> " << str << std::endl;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handle_close); // User pressed Ctrl + C

    auto obs = volt::event::observer<volt::reader_ptr>(msg_event);

    std::cout << "Server launched" << std::endl;
    {
        auto net_listener = volt::listener(64420);

        while (!quit)
        {
            // std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            volt::net::notify_listeners();
        }
    }
    return 0;
}
