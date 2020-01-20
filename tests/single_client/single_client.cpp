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
    std::string user     = "";
    std::string chat_msg = "";
};

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

int main(int argc, char *argv[])
{
    signal(SIGINT, handle_close); // User pressed Ctrl + C

    std::string username;
    std::cout << "Username: ";
    std::cin >> username;

    auto network = volt::net::network::create(8, false);
    network->set_new_msg_callback([](volt::net::reader_ptr reader) {
        test t = test();
        reader->read_msg(t);
        std::cout << t.user << ": " << t.chat_msg << std::endl;
    });

    auto con_status = network->server_connect("127.0.0.1", "64430");
    int  dot_count  = 1;
    while (con_status->GetState() ==
               volt::net::ServerConnectStatus::Connecting &&
           !error_quit)
    {
        // Wait until something actually happens
        std::cout << "\rConnecting";
        for (int i = 1; i <= 3; i++)
            std::cout << ((i <= dot_count) ? "." : " ");
        std::cout << std::flush;
        dot_count = (dot_count % 3) + 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (con_status->GetState() == volt::net::ServerConnectStatus::Connected)
    {
        std::cout << "Client connected" << std::endl;
    }
    else if (con_status->GetState() == volt::net::ServerConnectStatus::Failed)
    {
        std::cout << "Connection attempt failed" << std::endl;
        error_quit = true;
        std::exit(1);
    }

    while (!error_quit)
    {
        std::string chat_msg = "";
        std::getline(std::cin, chat_msg);

        if (chat_msg == "quit" || chat_msg == "exit")
        {
            error_quit = true;
            break;
        }

        {
            auto net_lock = network->get_guard();
            if (network->get_con_count(net_lock) > 0)
            {
                test t     = test();
                t.user     = username;
                t.chat_msg = chat_msg;

                auto writer = volt::net::make_writer();
                writer->write_msg(t);
                writer->end_msg();

                network->get_con_index(0, net_lock)
                    ->send_msg(writer->get_msg());
            }
            else
            {
                error_quit = true;
                break;
            }
        }

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
