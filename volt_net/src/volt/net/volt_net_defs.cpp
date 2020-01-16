#include "volt/net/volt_net_defs.hpp"

#include "volt/net/messages/msg_pool.hpp"

using namespace volt::net;

message_ptr volt::net::make_message()
{
    // When destroyed, the raw message pointer will be given to msg_pool, which
    // will then decide what will finally happen with the message. Whether it
    // should be pooled, or deleted.
    return std::shared_ptr<std::vector<net_word>>(
        new std::vector<net_word>(),
        [=](std::vector<net_word> *msg) { msg_pool::return_message(msg); });
}