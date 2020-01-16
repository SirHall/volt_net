#include "volt/net.hpp"

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <signal.h>
#include <string>
#include <thread>

#pragma region Network Test Data

struct test
{
    std::string user     = "";
    std::string chat_msg = "";
};

// This is the object we will send over the network

template <>
void volt::net::serialize::write_into(struct test const &v, message_ptr &data)
{
    volt::net::serialize::write_into(v.user, data);
    volt::net::serialize::write_into(v.chat_msg, data);
}

template <>
void volt::net::deserialize::read_into(message_iter &iterator,
                                       struct test & instance)
{
    volt::net::deserialize::read_into(iterator, instance.user);
    volt::net::deserialize::read_into(iterator, instance.chat_msg);
}

#pragma endregion

static std::atomic_bool error_quit = false;

// static int        msgs_recieved = 0;
// static std::mutex cnt_mut;

void handle_close(int s)
{
    std::cout << "\n\tForcefully Closing server (" << s << ")" << std::endl;
    error_quit = true;
}

void send_all(test &t)
{
    // auto writer =
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handle_close); // User pressed Ctrl + C

    auto network = volt::net::network::create(8, false);
    // When we receive a message, reflect it to all clients
    network->set_new_msg_callback(std::bind(
        [](volt::net::reader_ptr             reader,
           std::weak_ptr<volt::net::network> net_weak) {
            test t = test();
            reader->read_msg(t);
            std::cout << t.user << ": " << t.chat_msg << std::endl;

            auto send_writer = volt::net::make_writer();
            send_writer->write_msg(t);
            send_writer->end_msg();

            if (auto net = net_weak.lock())
            {
                auto net_lock = net->get_guard();
                for (std::size_t i = 0; i < net->get_con_count(net_lock); i++)
                {
                    net->get_con_index(i, net_lock)
                        ->send_msg(send_writer->get_msg());
                }
            }
        },
        std::placeholders::_1, network->get_net_weak()));

    std::cout << "Server launched" << std::endl;

    {
        auto net_lock = network->get_guard();
        network->start_listening(64430, 64431, net_lock);
    }

    while (!error_quit)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        network->process_new_msgs();
    }

    error_quit = true; // Just incase
    {
        auto net_lock = network->get_guard();
        network->close(net_lock);
    }
    std::cout << "End of main()" << std::endl;
    return 0;
}
