#include "volt/net/event_types/e_received_msg.hpp"

using namespace volt::net;

e_received_msg::e_received_msg(message_ptr message) : msg(std::move(message)) {}

// e_received_msg::e_received_msg(const e_received_msg &other) {}
// e_received_msg &e_received_msg::operator=(const e_received_msg &other){return
// *this;}

e_received_msg::e_received_msg(e_received_msg &&other)
{
    this->msg = std::move(other.msg);
}

e_received_msg &e_received_msg::operator=(e_received_msg &&other)
{
    this->msg = std::move(other.msg);
    return *this;
}

e_received_msg::~e_received_msg() {}

reader_ptr e_received_msg::get_reader() { return make_reader(); }
