#pragma once
#ifndef protocol_hpp
#define protocol_hpp

#include <string>

namespace volt::protocol
{
    class protocol
    {
      public:
        virtual ~protocol() = 0;

        virtual unsigned char prot_id();

        virtual void initialize();
    };
} // namespace volt::protocol

#endif