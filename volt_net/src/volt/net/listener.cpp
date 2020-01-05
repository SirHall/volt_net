#include "volt/net/listener.hpp"

#include <boost/bind.hpp>

using namespace volt::net;

// void volt::listener::loop()
// {
//     while (!this->io->stopped())
//         this->io->run();
// }

listener::listener(std::uint16_t hostport_ipv4, std::uint16_t hostport_ipv6,
                   std::function<void(tcp::socket)>         callback,
                   std::shared_ptr<boost::asio::io_context> io_context)
    : io(io_context),
      ipv4_acceptor(*io_context, tcp::endpoint(tcp::v4(), hostport_ipv4)),
      ipv6_acceptor(*io_context, tcp::endpoint(tcp::v6(), hostport_ipv6)),
      new_con_callback(callback), new_sock_ipv4(*this->io),
      new_sock_ipv6(*this->io)
{
    this->ipv4_acceptor.async_accept(
        new_sock_ipv4, boost::bind(&listener::accept_v4, this,
                                   boost::asio::placeholders::error));
    this->ipv6_acceptor.async_accept(
        new_sock_ipv6, boost::bind(&listener::accept_v6, this,
                                   boost::asio::placeholders::error));

    this->listener_open =
        this->ipv4_acceptor.is_open() && this->ipv6_acceptor.is_open();
}

listener::~listener() { this->close_listener(); }

bool listener::is_open() { return listener_open; }

// void volt::listener::listen() {}

void listener::accept_v4(const boost::system::error_code &err)
{
    this->accept_new_connection(err, std::move(new_sock_ipv4));
    this->new_sock_ipv4 = tcp::socket(*this->io);

    ipv4_acceptor.async_accept(new_sock_ipv4,
                               boost::bind(&listener::accept_v4, this,
                                           boost::asio::placeholders::error));
}

void listener::accept_v6(const boost::system::error_code &err)
{
    this->accept_new_connection(err, std::move(new_sock_ipv6));
    this->new_sock_ipv6 = tcp::socket(*this->io);

    ipv6_acceptor.async_accept(new_sock_ipv6,
                               boost::bind(&listener::accept_v6, this,
                                           boost::asio::placeholders::error));
}

void listener::accept_new_connection(const boost::system::error_code &err,
                                     tcp::socket                      socket)
{
    if (!listener_open || err)
        return;

    // Create a new connection and give it back
    this->new_con_callback(std::move(socket));

    // auto lock = this->network->get_guard();
    // this->network->add_connection(
    //     net_con::new_connection(this->io, std::move(socket)), lock);
}

void listener::close_listener()
{
    if (!is_open())
        return;
    listener_open = false;
    this->ipv4_acceptor.cancel();
    this->ipv4_acceptor.close();
    this->ipv6_acceptor.cancel();
    this->ipv6_acceptor.close();
}
