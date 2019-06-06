#pragma once
#ifndef defs_hpp
#define defs_hpp

// This must be equal across the entire program, servers and clients
namespace volt
{
    constexpr unsigned long buffer_size = 2048;
    // Must be large enough to contain buffer_size
    typedef unsigned short buffer_head;
    typedef unsigned char  net_word;

    // In network messages, the type used to specify the size of an array
    typedef unsigned int message_array_size;

    typedef volt::message::const_iterator message_iter;
} // namespace volt

#endif
