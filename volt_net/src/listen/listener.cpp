#include "volt/listen/listener.hpp"

void volt::listener::loop()
{
    while (listener_open)
    {
        accept_new_connection();
    }
}

int volt::listener::open_socket()
{
    // Get the socket file descriptor for the new socket
    socket_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

    int reuse_op     = 1;
    int ipv6_only_op = 0;
    // When destroying this socket we want it to instantly free so we can re-use
    // it on the next server launch
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_op, sizeof(int));
    // Allow connections from both IPv6 and IPv4
    setsockopt(socket_fd, SOL_SOCKET, IPV6_V6ONLY, &ipv6_only_op, sizeof(int));

    // Setup polling so we can check for new connections

    addr = std::unique_ptr<sockaddr>((sockaddr *)new sockaddr_in6());

    // Addr settings
    ((sockaddr_in6 *)addr.get())->sin6_family = AF_INET6; // TCP
    ((sockaddr_in6 *)addr.get())->sin6_addr   = in6addr_any;
    // This is modified again below
    ((sockaddr_in6 *)addr.get())->sin6_port = htons(port);

    int b_res = bind(socket_fd, (sockaddr *)addr.get(), sizeof(sockaddr_in6));

    // Bind this file descriptor to a port
    if (b_res < 0)
    {
        std::cerr << b_res << " Failed to bind port " << port << " to socket"
                  << std::endl;
        close(socket_fd);
        return -1;
        // TODO: Handle error
    }
    else if (listen(socket_fd, backlog) == -1)
    {
        std::cerr << "Failed to listen on " << socket_fd << std::endl;
        close(socket_fd);
        return -1;
        // TODO: Handle error
    }

    std::cout << "Successfully bound to port " << this->port << std::endl;

    return 0;
}

volt::listener::listener(std::uint16_t hostport)
{
    this->port = hostport;
    if (open_socket() == 0)
    {
        fds->fd     = socket_fd;
        fds->events = 0; // Just for that extra layer of safety
        fds->events = POLLIN;

        thr           = std::thread([=]() { this->loop(); });
        listener_open = true;
    }
    else
    {
        listener_open = false;
    }
}

volt::listener::~listener()
{
    close_listener();
    thr.join();
}

bool volt::listener::is_open() { return listener_open; }

void volt::listener::accept_new_connection()
{
    if (!listener_open)
        return;

    auto poll_res = poll(fds, 1, timeout);

    if (poll_res < 0)
    { // Error occured
        listener_open = false;
        return;
    }
    else if (poll_res > 0)
    { // New connection
        auto new_addr =
            std::unique_ptr<sockaddr>((sockaddr *)new sockaddr_in6());
        socklen_t len = sizeof(sockaddr_in6);

        std::cout << "\t\tWaiting for new connection on " << socket_fd
                  << std::endl;
        int con_fd = accept(socket_fd, new_addr.get(), &len);
        std::cout << "Accepted new connection on file descriptor of " << con_fd
                  << std::endl;

        if (con_fd < 0)
        {
            std::cerr << "Error accepting new con on file descriptor " << con_fd
                      << std::endl;
        }
        else
        {
            auto lock = volt::net_con::aquire_lock();
            volt::net_con::new_connection(con_fd, lock);
        }
    }
}

void volt::listener::close_listener()
{
    if (!is_open())
        return;
    std::cout << "Closing listener" << std::endl;
    listener_open = false;
    close(socket_fd);
}