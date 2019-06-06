#include <iostream>

#include "volt/listen/listener.hpp"
#include "volt/message.hpp"
#include "volt/serialization.hpp"

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

    test t = test();

    auto m = volt::message();

    volt::serialize::write_into(t, m);

    // m.write(t);

    test new_t; //= *volt::deserialize::read_new<test>(m.begin() + m.size());

    auto it = m.begin();

    volt::deserialize::read_into(it, new_t);

    std::cout << new_t.a << " " << new_t.b << " " << new_t.c << std::endl;

    // volt::serialize::write_into();

    // auto l = volt::listener::listener();

    // while (true)
    // {
    //     std::cout << (l.new_connection() ? "true" : "false") << std::endl;
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // }

    return 0;
}
