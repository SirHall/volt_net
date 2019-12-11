#include "volt/net.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <signal.h>
#include <string>
#include <thread>

#pragma region Network Test Data

struct test
{
    int         a = 1;
    double      b = 3.141592658;
    char        c = 'c';
    std::string s = "!A basic test string!";
    bool        operator==(const struct test &rhs)
    {
        return (a == rhs.a) && (c == rhs.c) && (s == rhs.s);
    }
};

// This is the object we will send over the network
static struct test send_val = test();

template <>
void volt::net::serialize::write_into(struct test const &v, message_ptr &data)
{
    volt::net::serialize::write_into(v.a, data);
    // volt::net::serialize::write_into(v.b, data);
    volt::net::serialize::write_into(v.c, data);
    volt::net::serialize::write_into(v.s, data);
}

template <>
void volt::net::deserialize::read_into(message_iter &iterator,
                                       struct test & instance)
{
    volt::net::deserialize::read_into(iterator, instance.a);
    // volt::net::deserialize::read_into(iterator, instance.b);
    volt::net::deserialize::read_into(iterator, instance.c);
    volt::net::deserialize::read_into(iterator, instance.s);
}

#pragma endregion

static std::atomic_bool error_quit = false;

static int        msgs_recieved = 0;
static std::mutex cnt_mut;

void handle_close(int s)
{
    std::cout << "\n\tForcefully Closing server (" << s << ")" << std::endl;
    error_quit = true;
}

void msg_event(volt::net::reader_ptr const &reader)
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

    // Setup send_val
    send_val.a = -104;
    // send_val.b = 1023.2134;
    send_val.c = 'p';
    send_val.s = "Msg sent correctly";

    auto network = volt::net::network::create(8, true);
    network->set_new_msg_callback([](volt::net::reader_ptr reader) {
        test t = test();
        reader->read_msg(t);
        std::cout << "a: " << t.a << std::endl;
        // std::cout << "b: " << t.b << std::endl;
        std::cout << "c: " << t.c << std::endl;
        std::cout << "s: " << t.s << std::endl;
        std::cout << std::endl;
    });

    std::cout << "Server launched" << std::endl;

    {
        auto net_lock = network->get_guard();
        network->start_listening(64430, 64431, net_lock);
    }

    // Instantiate 10 clients
    for (int i = 0; (i < 10) && !error_quit; i++)
    {
        auto con_status = network->server_connect("127.0.0.1", "64430");
        int  dot_count  = 1;
        while (con_status->GetState() ==
                   volt::net::ServerConnectStatus::Connecting &&
               !error_quit)
        {
            // Wait until something actually happens
            // std::cout << "\rConnecting"; //<< std::endl;
            // for (int i = 1; i <= 3; i++)
            //     std::cout << ((i <= dot_count) ? "." : " ");
            // std::cout << std::flush;
            dot_count = (dot_count % 3) + 1;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (con_status->GetState() == volt::net::ServerConnectStatus::Connected)
        {
            std::cout << "Client connected" << std::endl;
        }
        else if (con_status->GetState() ==
                 volt::net::ServerConnectStatus::Failed)
        {
            std::cout << "Connection attempt failed" << std::endl;
        }
    }

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    {

        for (std::size_t send_i = 0; (send_i < 10) && !error_quit; send_i++)
        {
            // Send data on connections
            auto writer = volt::net::msg_writer();

            writer.write_msg(send_val);

            {
                auto net_lock = network->get_guard();

                for (unsigned int i = 0; i < network->get_con_count(net_lock);
                     i++)
                {
                    if (auto &con = network->get_con_index(i, net_lock))
                    {
                        con->send_msg(writer.get_msg());
                    }
                    else
                    {
                        std::cerr << "Connection that should exist, doesn't"
                                  << std::endl;
                        return 1;
                    }
                }
            }

            // {
            //     auto lock = std::lock_guard(cnt_mut);
            //     if (msgs_recieved >= 100)
            //     {
            //         std::cout << "Test succeeded, all data transfered
            //         correctly"
            //                   << std::endl;
            //         break;
            //     }
            //     if (error_quit)
            //         break;
            // }
            // if (error_quit)
            // {
            //     std::cerr << "Error occurred, forcefully closing the server"
            //               << std::endl;
            //     return 1;
            // }

            network->process_new_msgs();
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    network->process_new_msgs();

    error_quit = true; // Just incase
    {
        auto net_lock = network->get_guard();
        network->close(net_lock);
    }
    std::cout << "End of main()" << std::endl;
    return 0;
}
