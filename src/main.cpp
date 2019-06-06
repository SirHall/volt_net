#include <iostream>

#include "volt/listen/listener.hpp"

#include <chrono>
#include <thread>

int main(int argc, char *argv[])
{
    auto l = volt::listener::listener();

    while (true)
    {
        std::cout << (l.new_connection() ? "true" : "false") << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}
