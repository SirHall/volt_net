#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "volt/global_events/global_event.hpp"
#include "volt/global_events/observer.hpp"
using namespace volt::event;

class test_data
{
  public:
    test_data() { std::cout << "created" << std::endl; }
    test_data(test_data &other) = delete;
    ~test_data() { std::cout << "destroyed" << std::endl; }
    std::string str = "thing";
};

void print_event(test_data &inst) { std::cout << inst.str << std::endl; }

void print_tabbed_event(test_data &inst)
{
    std::cout << '\t' << inst.str << std::endl;
}

int main(int argc, char *argv[])
{
    auto obs       = observer<test_data>(print_event);
    auto obstabbed = observer<test_data>(print_tabbed_event);

    {
        auto e      = global_event<test_data>();
        e.get().str = "Hello there world!";
    }

    {
        auto e      = global_event<test_data>();
        e.get().str = "Hello2";
    }

    return 0;
}
