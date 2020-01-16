#pragma once
#ifndef volt_net_defs_hpp
#define volt_net_defs_hpp

#include <cstdint>
#include <memory>
#include <vector>

#define _XOPEN_SOURCE_EXTENDED 1

// This must be identical across the entire program, servers and clients
namespace volt::net
{
    typedef std::uint16_t buffer_size;
    constexpr buffer_size max_buffer_size = 2048;
    // Must be large enough to contain buffer_size
    typedef std::uint16_t buffer_head;
    typedef std::uint8_t  net_word; // DO NOT CHANGE

    constexpr net_word escape_val      = 255;
    constexpr net_word msg_end_escaped = 0;
    constexpr net_word msg_origi_char  = 255;

    typedef std::uint16_t net_tag;

    // In network messages, the type used to specify the size of an array
    typedef std::uint32_t message_array_size;

    typedef std::vector<net_word>::iterator        message_iter;
    typedef std::vector<net_word>                  message_t;
    typedef std::shared_ptr<std::vector<net_word>> message_ptr;

    // Returns a new instance of a message pointer
    message_ptr make_message();

    // Connection ID's
    typedef std::uint16_t con_id;
} // namespace volt::net

#endif
