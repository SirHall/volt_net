#include "volt/messages/msg_writer.hpp"

using namespace volt::net;

msg_writer::msg_writer(message_ptr message) { msg = std::move(message); }

msg_writer::msg_writer() { msg = std::move(msg_pool::get_message()); }

msg_writer::~msg_writer()
{
    msg->resize(0);
    msg_pool::return_message(std::move(msg));
}

message_ptr &msg_writer::get_msg() { return std::ref(msg); }

writer_ptr make_writer() { return std::make_unique<msg_writer>(); }

writer_ptr make_writer(message_ptr msg)
{
    return std::make_unique<msg_writer>(std::move(msg));
}