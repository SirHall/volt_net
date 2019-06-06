#pragma once
#ifndef tcp_protocol_hpp
#define tcp_protocol_hpp

#include "volt/protocols/protocol.hpp"

namespace volt::protocol
{
    class tcp_protocol : public protocol
    {
      public:
        tcp_protocol()  = default;
        ~tcp_protocol() = default;
    };
} // namespace volt::protocol

#endif