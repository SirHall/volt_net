#pragma once
#ifndef protocol_hpp
#define protocol_hpp

#include "volt/defs.hpp"
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
        virtual ~protocol() = 0;

        virtual unsigned char prot_id();

        virtual void send_msg(volt::message &m);

        std::vector<volt::message> *recieve_msg();
    };
} // namespace volt::protocol

#endif