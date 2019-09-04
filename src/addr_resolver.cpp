#include "volt/addr_resolver.hpp"

void volt::net::addrinfo_deletor::operator()(struct addrinfo *addr)
{
    freeaddrinfo(addr);
}

volt::net::addrinfo_ptr volt::net::resolve_address(std::string const address,
                                                   std::string const port)
{
    // Setup for finding connection
    struct addrinfo preference = addrinfo();

    preference.ai_family   = AF_UNSPEC;   // Allow IPv4 & IPv6
    preference.ai_socktype = SOCK_STREAM; // Use TCP

    // Have to have this temporary field to prevent the lvalue error
    struct addrinfo *addrs_found_ptr = nullptr;

    int get_addr_status = getaddrinfo(address.c_str(), port.c_str(),
                                      &preference, &addrs_found_ptr);

    if (get_addr_status < 0)
        return addrinfo_ptr(nullptr);
    else
        return addrinfo_ptr(addrs_found_ptr);
}