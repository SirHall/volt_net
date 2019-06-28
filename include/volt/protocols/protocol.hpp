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
} // namespace volt

namespace volt::protocol
{
    class protocol
    {
      public:
        // virtual ~protocol() = 0;

        virtual volt::prot_identifier prot_id() = 0;

        virtual void send_msg(volt::message_ptr const &m) = 0;

        virtual volt::message_ptr recieve_msg() = 0;
    };
} // namespace volt::protocol

#endif