#include <iostream>

#include "volt/connection.hpp"
#include "volt/listen/listener.hpp"
#include "volt/message.hpp"
#include "volt/messages/msg_reader.hpp"
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

    auto new_cons = net_listener.accept_new_connection();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    volt::message_ptr new_msg = new_cons->at(0)->recieve_messages(0);

    auto reader = volt::msg_reader(std::move(new_msg));

    std::int64_t content = 0;

    reader.read_msg_int<std::int64_t>(content);

    std::cout << "Recieved message: " << content << std::endl;

    return 0;
}
