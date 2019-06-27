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

int main(int argc, char *argv[]) { return 0; }
