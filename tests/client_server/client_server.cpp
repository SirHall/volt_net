#include <iostream>

#include "volt/global_events/observer.hpp"
#include "volt/listen/listener.hpp"
#include "volt/messages/msg_reader.hpp"
#include "volt/messages/msg_rec_pool.hpp"
#include "volt/net_con.hpp"
#include "volt/serialization/net_serialize.hpp"
#include "volt/serialization/serialization_ext.hpp"

#include <atomic>
#include <chrono>
#include <signal.h>
#include <string>
#include <thread>

using namespace volt;
using namespace volt::serialize;
using namespace volt::deserialize;

#pragma region Network Test Data

struct test
{
    int a = 1;
    // double      b = 3.141592658;
    char        c = 'c';
    std::string s = "!A basic test string!";
    bool        operator==(const struct test &rhs)
    {
        return (a == rhs.a) && (c == rhs.c) && (s == rhs.s);
    }
};

// This is the object we will send over the network
static struct test send_val = test();

namespace volt::serialize
{
    template <>
    void volt::serialize::write_into(struct test const &v,
                                     volt::message_ptr &data)
    {
        write_into(v.a, data);
        // write_into(v.b, data);
        write_into(v.c, data);
        write_into(v.s, data);
    }
} // namespace volt::serialize

namespace volt::deserialize
{
    template <>
    void volt::deserialize::read_into(message_iter &iterator,
                                      struct test & instance)
    {
        read_into(iterator, instance.a);
        // read_into(iterator, instance.b);
        read_into(iterator, instance.c);
        read_into(iterator, instance.s);
    }
} // namespace volt::deserialize

#pragma endregion

static std::atomic_bool error_quit = false;

static int        msgs_recieved = 0;
static std::mutex cnt_mut;

void handle_close(int s)
{
    std::cout << "\n\tForcefully Closing server (" << s << ")" << std::endl;
    error_quit = true;
}

void msg_event(volt::reader_ptr const &reader)
{
    struct test rec_val; // Recieve this from the network
    reader->read_msg(rec_val);
    // Ensure that all the data is exactly the same
    error_quit = rec_val == send_val;

    {
        auto lock = std::lock_guard(cnt_mut);
        msgs_recieved++;
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handle_close); // User pressed Ctrl + C

    auto obs = volt::event::observer<volt::reader_ptr>(msg_event);

    std::cout << "Server launched" << std::endl;

    // Instantiate 10 clients

    {
        auto net_listener = volt::listener(64420);
        if (!net_listener.is_open())
        {
            std::cerr << "Failed to open listener" << std::endl;
            return 1;
        }

        for (int i = 0; i < 10; i++)
            volt::net_con::server_connect("localhost", "64420");

        while (true)
        {
            // Send data on connections
            auto writer = volt::msg_writer();

            writer.write_msg(send_val);

            {
                auto lock = volt::net_con::aquire_lock();

                for (unsigned int i = 0; i < volt::net_con::con_count(lock);
                     i++)
                {
                    if (auto &con = volt::net_con::get_con(i, lock))
                    {
                        if (!con->send_msg(writer.get_msg()))
                        {
                            std::cerr << "Failed to send data over connection"
                                      << std::endl;
                            return 1;
                        }
                    }
                    else
                    {
                        std::cerr << "Connection that should exist, doesn't"
                                  << std::endl;
                        return 1;
                    }
                }
            }

            {
                auto lock = std::lock_guard(cnt_mut);
                if (msgs_recieved >= 100)
                {
                    std::cout << "Test succeeded, all data transfered correctly"
                              << std::endl;
                    break;
                }
            }
            if (error_quit)
            {
                std::cerr << "Error occurred, forcefully closing the server"
                          << std::endl;
                return 1;
            }

            // std::this_thread::sleep_for(std::chrono::milliseconds(500));
            volt::net::notify_listeners();
        }
    }
    return 0;
}
