#include "volt/net/volt_net_defs.hpp"

#include "volt/net/messages/msg_pool.hpp"

using namespace volt::net;

message_ptr volt::net::make_message()
{
    return std::make_shared<std::vector<net_word>>();
}
