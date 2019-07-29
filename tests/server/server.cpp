#include <iostream>

#include "volt/global_events/observer.hpp"
#include "volt/listen/listener.hpp"
#include "volt/messages/msg_reader.hpp"
#include "volt/messages/msg_rec_pool.hpp"
#include "volt/net_con.hpp"
#include "volt/serialization/serialization_ext.hpp"

#include <atomic>
#include <chrono>
#include <execinfo.h>
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

// Borrowed kindly from https://stackoverflow.com/a/77336
void handle_segfault(int sig)
{
    void * array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

void msg_event(volt::reader_ptr const &reader)
{
    std::string str = "";
    reader->read_msg(str);
    std::cout << "\t\t> " << str << std::endl;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handle_close);     // User pressed Ctrl + C
    signal(SIGSEGV, handle_segfault); // Segmentation fault

    auto obs = volt::event::observer<volt::reader_ptr>(msg_event);

    std::cout << "Server launched" << std::endl;

    auto net_listener = volt::listener();

    while (!quit)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        volt::net::notify_listeners();
    }

    return 0;
}
