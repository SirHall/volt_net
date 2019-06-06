#pragma once
#ifndef connection_hpp
#define connection_hpp

#include "volt/message.hpp"
#include <memory>
#include <vector>

namespace volt::protocol
{
    class protocol;
}

namespace volt
{
    class connection
    {
      private:
        std::vector<std::unique_ptr<volt::protocol::protocol>> protocols;

      public:
        // Creates a new connection given the TCP file descriptor
        connection(int con_fd) {}

        ~connection() {}

        // TODO: Find some clean way to specify which protocol to use
        void send_message(message &m) {}
    };
} // namespace volt

#endif
