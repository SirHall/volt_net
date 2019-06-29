#pragma once
#ifndef defs_hpp
#define defs_hpp

#include <cstdint>
#include <memory>
#include <vector>

// This must be identical across the entire program, servers and clients
namespace volt
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
    typedef std::unique_ptr<std::vector<net_word>> message_ptr;

    // Returns a new instance of a message pointer
    message_ptr make_message()
    {
        return std::make_unique<std::vector<net_word>>();
    }

    typedef std::uint8_t prot_identifier;

    // Connection ID's
    typedef std::uint_fast32_t con_id;

    class msg_reader;
    class msg_writer;

    typedef std::unique_ptr<msg_reader> reader_ptr;
    typedef std::unique_ptr<msg_writer> writer_ptr;

    reader_ptr make_reader() { return std::make_unique<msg_reader>(); }
    reader_ptr make_reader(message_ptr msg)
    {
        return std::make_unique<msg_reader>(std::move(msg));
    }

    writer_ptr make_writer() { return std::make_unique<msg_writer>(); }
    writer_ptr make_writer(message_ptr msg)
    {
        return std::make_unique<msg_writer>(std::move(msg));
    }

} // namespace volt

#endif
