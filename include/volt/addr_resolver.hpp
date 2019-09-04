#pragma once
#ifndef addr_resolver_hpp
#define addr_resolver_hpp

// For addrinfo to work nicely
// #define _POSIX_C_SOURCE 200112L

#include <memory>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

namespace volt::net
{
    struct addrinfo_deletor
    {
        void operator()(struct addrinfo *addr);
    };

    typedef std::unique_ptr<struct addrinfo, addrinfo_deletor> addrinfo_ptr;

    // Given an address and port will attempt to resolve an address structor.
    // Will return the address structure as a smart pointer. Use inside if()
    // statement to test success.
    addrinfo_ptr resolve_address(std::string const address,
                                 std::string const port);
} // namespace volt::net

#endif
