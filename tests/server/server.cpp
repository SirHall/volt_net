#include <iostream>

#include "volt/connection.hpp"
#include "volt/listen/listener.hpp"
#include "volt/message.hpp"
#include "volt/serialization.hpp"
#include "volt/serialization_ext.hpp"

#include <chrono>
#include <string>
#include <thread>

struct test
{
    int    a = 1;
    double b = 3.141592658;
    char   c = 'c';
};

int main(int argc, char *argv[])
{
    std::cout << "Server launched" << std::endl;

    std::vector<volt::connection> cons;

    auto net_listener = volt::listener::listener();

    // while (!net_listener.new_connection())
    // {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // }

    auto new_cons = net_listener.accept_new_connection();

    auto msg = volt::message();

    // auto content = std::vector<char>({'H', 'e', 'y', '\n'});
    // msg.write_array(content);

    // new_cons->at(0)->send_message(msg, 0);
    // std::cout << "Sent message to client" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    auto messages = new_cons->at(0)->recieve_messages(0);

    std::cout << "Recieved " << messages->size() << " messages" << std::endl;

    auto &new_msg = messages->at(0);
    new_msg->reset_iterator(); // Very important

    std::cout << "msg size: " << new_msg->size() << std::endl;

    auto msg_text = new_msg->read_new<std::string>();

    std::cout << *msg_text << std::endl;

    return 0;
}
