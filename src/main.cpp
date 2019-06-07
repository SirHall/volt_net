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
    auto array = std::vector({
        test{1, 1.2, '5'},
        test{2, 3.141592685, 'g'},
        test{3, 1.13721839, 'b'},
        test{4, 1.4, 'y'},
        test{5, 1.6, 'u'},
    });

    test t = test();

    auto m = volt::message(0);

    m.write<test>(t);

    m.write<test>(test{123, 4.567, 'a'});

    m.write_array(array);

    m.reset_iterator();
    test new_t; //= *volt::deserialize::read_new<test>(m.begin() + m.size());

    m.read_into<test>(new_t);
    std::cout << new_t.a << " " << new_t.b << " " << new_t.c << std::endl;
    m.read_into<test>(new_t);
    std::cout << new_t.a << " " << new_t.b << " " << new_t.c << std::endl;

    std::cout << std::endl;

    auto back_array = m.read_new_array<test>();

    for (auto e : *back_array)
        std::cout << e.a << " " << e.b << " " << e.c << std::endl;

    // volt::serialize::write_into();

    // auto l = volt::listener::listener();

    // while (true)
    // {
    //     std::cout << (l.new_connection() ? "true" : "false") <<
    //     std::endl;
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // }

    return 0;
}
