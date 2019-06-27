#pragma once
#ifndef endian_hpp
#define endian_hpp

#include "../../dep/endian_h/endian.h"

namespace volt::net
{
#include <cstdint>
    // Source: https://stackoverflow.com/a/1001373
    bool test_big_endian(void)
    {
        union {
            std::uint32_t i;
            char     c[4];
        } be_test = {0x01020304};

        return be_test.c[0] == 1;
    }

    //Is big endian
    // Ensure this does not get evaluated at compile time,
    // but at launch.
    volatile const bool is_be = test_big_endian();
} // namespace volt::net
#endif
