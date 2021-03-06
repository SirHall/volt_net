﻿#include "volt/net/messages/msg_reader.hpp"

using namespace volt::net;

msg_reader::msg_reader(message_ptr message) : msg(std::move(message))
{
    iter = msg->begin();
}

msg_reader::msg_reader() : msg(std::move(msg_pool::get_message()))
{
    iter = msg->begin();
}

msg_reader::~msg_reader()
{
    //  msg_pool::return_message(msg);
}

// Global
reader_ptr volt::net::make_reader() { return std::make_unique<msg_reader>(); }

reader_ptr volt::net::make_reader(message_ptr const msg)
{
    return std::make_unique<msg_reader>(std::move(msg));
}
