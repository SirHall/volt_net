#pragma once
#ifndef msg_writer_hpp
#define msg_writer_hpp

#include "volt/messages/msg_send.hpp"
#include "volt/serialization.hpp"
#include <memory>

namespace volt::net::msg
{
    class msg_writer
    {
      private:
        std::unique_ptr<msg_send> msg;

      public:
        msg_writer()
        {
            // TODO: Get msg_send instance from central queue
            msg = make_unique<msg_send>();

            msg->resize(0);
        }
        ~msg_writer()
        {
            // TODO: Give the message back to the central queue

            volt::serialize::write_end();
        }
    }
} // namespace volt::net::msg

#endif
