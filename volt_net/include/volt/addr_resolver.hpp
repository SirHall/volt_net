#pragma once
#ifndef addr_resolver_hpp
#define addr_resolver_hpp

// For addrinfo to work nicely
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif

#include <memory>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

namespace volt::net
{
    /**
     * @brief A functor that frees a addrinfo struct from memory
     *
     */
    struct addrinfo_deletor
    {
        /**
         * @brief Frees any passes addrinfo struct from memory
         *
         * @param addr
         */
        void operator()(struct addrinfo *addr);
    };

    typedef std::unique_ptr<struct addrinfo, addrinfo_deletor> addrinfo_ptr;

    /**
     * @brief     Given an address and port will attempt to resolve an address
     * structor. Will return the address structure as a smart pointer. Use
     * inside an if() statement to test success
     *
     * @param address The address to attempt an address resolution for
     * @param port The port to search
     * @return addrinfo_ptr The resulting addrinfo struct wrapped in a smart
     * pointer with custom destructor
     */
    addrinfo_ptr resolve_address(std::string const address,
                                 std::string const port);
} // namespace volt::net

#endif
