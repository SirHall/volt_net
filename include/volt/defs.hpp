#pragma once
#ifndef defs_hpp
#define defs_hpp

#include <vector>

// This must be equal across the entire program, servers and clients
namespace volt
{
    constexpr unsigned long buffer_size = 2048;
    // Must be large enough to contain buffer_size
    typedef unsigned short  buffer_head;
    typedef unsigned char   net_word;
    constexpr unsigned long buffer_mem_size = sizeof(net_word) * buffer_size;

    typedef unsigned short net_tag;

    // In network messages, the type used to specify the size of an array
    typedef unsigned int message_array_size;

    typedef std::vector<net_word>::iterator message_iter;

    typedef unsigned char prot_identifier;
} // namespace volt

#endif
