#pragma once
#ifndef protocol_hpp
#define protocol_hpp

#include "volt/defs.hpp"
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

namespace volt
{
    class message;
}

namespace volt::protocol
{
    class protocol
    {
      public:
        // virtual ~protocol() = 0;

        virtual volt::prot_identifier prot_id() = 0;

        virtual void send_msg(volt::message const &m) = 0;

        virtual std::unique_ptr<std::vector<std::unique_ptr<volt::message>>>
            recieve_msg() = 0;
    };
} // namespace volt::protocol

#endif