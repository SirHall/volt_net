#pragma once
#ifndef connection_hpp
#define connection_hpp

#include <memory>
#include <vector>

namespace volt::protocol
{
    class protocol;
} // namespace volt::protocol

namespace volt
{
    class message;

    class connection
    {
      private:
        std::vector<std::unique_ptr<volt::protocol::protocol>> protocols;

      public:
        // Creates a new connection given the TCP file descriptor
        connection() {}

        ~connection() {}

        void add_protocol(std::unique_ptr<volt::protocol::protocol> prot)
        {
            protocols.push_back(std::move(prot));
        }

        // TODO: Find some clean way to specify which protocol to use
        void send_message(message &m) {}
    };
} // namespace volt

#endif
