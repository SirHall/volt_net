#include "volt/messages/msg_writer.hpp"

volt::msg_writer::msg_writer(volt::message_ptr message)
{
    msg = std::move(message);
}

volt::msg_writer::msg_writer()
{
    msg = std::move(volt::msg_pool::get_message());
}

volt::msg_writer::~msg_writer()
{
    msg->resize(0);
    volt::msg_pool::return_message(std::move(msg));
}

volt::message_ptr &volt::msg_writer::get_msg() { return std::ref(msg); }

volt::writer_ptr volt::make_writer() { return std::make_unique<msg_writer>(); }

volt::writer_ptr volt::make_writer(message_ptr msg)
{
    return std::make_unique<msg_writer>(std::move(msg));
}