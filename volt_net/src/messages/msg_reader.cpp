#include "volt/messages/msg_reader.hpp"

volt::msg_reader::msg_reader(volt::message_ptr message)
{
    msg  = std::move(message);
    iter = msg->begin();
}

volt::msg_reader::msg_reader()
{
    msg  = std::move(volt::msg_pool::get_message());
    iter = msg->begin();
}

volt::msg_reader::~msg_reader()
{
    volt::msg_pool::return_message(std::move(msg));
}

// Global
volt::reader_ptr volt::make_reader() { return std::make_unique<msg_reader>(); }

volt::reader_ptr volt::make_reader(message_ptr msg)
{
    return std::make_unique<msg_reader>(std::move(msg));
}