﻿#include "volt/net/messages/msg_writer.hpp"

using namespace volt::net;

msg_writer::msg_writer(message_ptr message) { msg = message; }

msg_writer::msg_writer() { msg = msg_pool::get_message(); }

msg_writer::~msg_writer()
{
    // msg->resize(0);
    // msg_pool::return_message(msg);
}

void msg_writer::end_msg() { volt::net::serialize::end_msg(this->msg); }

message_ptr msg_writer::get_msg() { return msg; }

writer_ptr volt::net::make_writer() { return std::make_unique<msg_writer>(); }

writer_ptr volt::net::make_writer(message_ptr msg)
{
    return std::make_unique<msg_writer>(msg);
}
