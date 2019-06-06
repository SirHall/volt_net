#pragma once
#ifndef listener_hpp
#define listener_hpp

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <poll.h>

class listener
{
  private:
    sockaddr_in addr;
    int         socket_fd = 0;
    uint16_t    port      = 0;
    pollfd      poll_fd;
    int         backlog = 10;

    void open_socket()
    {
        // Get the socket file descriptor for the new socket
        socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        // Setup polling so we can check for new connections
        poll_fd.fd     = socket_fd;
        poll_fd.events = POLLOUT | POLLWRBAND;

        // Addr settings
        addr.sin_family      = AF_INET; // TCP
        addr.sin_port        = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        // Bind this file descriptor to a port
        if (bind(socket_fd, (sockaddr *)&addr, sizeof(addr)) < 0)
        {
            // TODO: Handle error
        }

        if (listen(socket_fd, backlog) == -1)
        {
            // TODO: Handle error
        }
    }

  public:
    listener() { open_socket(); }
    ~listener() {}

    // Check for any new connection requests
    // returns: true = new connection waiting
    bool new_connection()
    {
        poll(&poll_fd, 1, 0);
        return poll_fd.revents & (POLLWRBAND | POLLOUT);
    }

    // Accepts the new connection
    // return: new connection file descriptor. -1 on failure
    int accept_new_connection()
    {
        // TODO: Actually store and return the addr structure and addr length
        int conn_fd = accept(socket_fd, nullptr, nullptr);
    }
};

#endif