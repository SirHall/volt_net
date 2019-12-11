#include "volt/net/volt_net_defs.hpp"

using namespace volt::net;

message_ptr volt::net::make_message()
{
    return std::make_unique<std::vector<net_word>>();
}