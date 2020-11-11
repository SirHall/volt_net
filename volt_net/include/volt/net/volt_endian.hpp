#pragma once
#ifndef voltendian_hpp
#define voltendian_hpp

#include <cstdint>

namespace volt::net
{
    // Source: https://stackoverflow.com/a/1001373
    constexpr bool is_big_endian()
    {
        union
        {
            std::uint32_t i;
            char          c[4];
        } be_test = {0x01020304};

        return be_test.c[0] == 1;
    }
} // namespace volt::net
#endif
