#include "volt/net/network.hpp"

#include <algorithm>

#include <boost/asio.hpp>
#include <boost/function.hpp>

using namespace volt::net;

network::network(std::size_t thread_count, bool defer_received_msgs)
    : running(true), io(std::make_shared<boost::asio::io_context>()),
      work(boost::asio::make_work_guard(io->get_executor())),
      net_listener(nullptr),
      received_msgs(defer_received_msgs, [=](con_id id, message_ptr msg) {
          if (this->new_msg_callback)
              this->new_msg_callback(id, make_reader(msg));
      })
{
    this->thr_pool.reserve(thread_count);

    for (std::size_t i = 0; i < thread_count; i++)
    {
        this->thr_pool.push_back(std::thread([=]() {
            if (auto io_lock = this->get_io().lock())
            {
                io_lock->run();
            }
        }));
    }
}

std::shared_ptr<network> network::create(std::size_t thread_count,
                                         bool        defer_received_msgs)
{
    auto net = std::shared_ptr<network>(
        new network(thread_count, defer_received_msgs));
    net->self = std::weak_ptr(net);
    return net;
}

network::~network()
{
    this->running.store(false);
    {
        auto con_lck = this->get_guard();
        for (auto &con : this->connections)
            con->close_con();
    }
    if (this->net_listener)
        this->net_listener->close_listener();
    this->work.reset();
    this->io->stop();
    for (auto &thr : this->thr_pool)
        thr.join();
}

std::lock_guard<std::recursive_mutex> network::get_guard()
{
    return std::lock_guard(this->connections_mut);
}

void network::add_connection(std::unique_ptr<net_con>                     con,
                             std::lock_guard<std::recursive_mutex> const &lck)
{
    con->set_new_msg_callback([&](con_id id, message_ptr msg) {
        auto net_lock = this->get_guard();
        this->received_msgs.submit_message(id, msg);
    });
    con->set_closed_callback([&](con_id id) {
        auto net_lock = this->get_guard();
        if (this->closed_connection_callback)
            this->closed_connection_callback(*con);
        if (!this->running.load())
            return; // We don't want to delete the net_con object twice
        auto con_pos = std::find_if(
            this->connections.begin(), this->connections.begin(),
            [=](connection_ptr &con) { return con->get_con_id() == id; });
        // Remove this connection from the list
        if (con_pos != this->connections.end())
            this->connections.erase(con_pos);
    });

    this->connections.push_back(std::move(con));
    if (this->new_connection_callback)
        this->new_connection_callback(*con);
}

connection_ptr &
    network::get_con(con_id                                       id,
                     std::lock_guard<std::recursive_mutex> const &lck)
{
    // Was having an issue with std::find_if() so am manually using a loop
    for (connection_ptr &con : this->connections)
        if (con->get_con_id() == id)
            return con;
    // No connection was found
    return this->null_con;
}

connection_ptr &
    network::get_con_index(std::size_t                                  index,
                           std::lock_guard<std::recursive_mutex> const &lck)
{
    return this->connections.at(index);
}

std::size_t
    network::get_con_count(std::lock_guard<std::recursive_mutex> const &lck)
{
    return this->connections.size();
}

std::shared_ptr<AsyncStateHandler<ServerConnectStatus>>
    network::server_connect(std::string const address, std::string const port)
{
    using namespace boost::asio::ip;
    auto state = AsyncStateHandler<ServerConnectStatus>::Create();
    state->SetState(ServerConnectStatus::Connecting);

    this->io->post(boost::bind(
        boost::function<void(
            std::string, std::string, std::weak_ptr<network>,
            std::shared_ptr<boost::asio::io_context>,
            std::shared_ptr<AsyncStateHandler<ServerConnectStatus>>)>(
            [](std::string address, std::string port,
               std::weak_ptr<network>                                  net,
               std::shared_ptr<boost::asio::io_context>                io,
               std::shared_ptr<AsyncStateHandler<ServerConnectStatus>> state) {
                //     tcp::resolver::query resolver_query(
                //         address, port,
                //         tcp::resolver::query::numeric_service);
                boost::system::error_code   err;
                tcp::resolver               resolver(*io);
                tcp::resolver::results_type endpoints =
                    resolver.resolve(address, port, err);

                if (err)
                {
                    state->SetState(ServerConnectStatus::Failed);
                    return;
                }

                auto new_sock = tcp::socket(*io);
                boost::asio::connect(new_sock, endpoints, err);

                if (err)
                {
                    state->SetState(ServerConnectStatus::Failed);
                    return;
                }

                if (auto lock = net.lock())
                {
                    auto con = net_con::new_connection(std::move(new_sock), io,
                                                       lock->get_next_id());
                    auto net_lock = lock->get_guard();
                    lock->add_connection(std::move(con), net_lock);
                }

                state->SetState(ServerConnectStatus::Connected);
            }),
        address, port, this->self, io, state));

    return state;
}

bool network::is_running() { return this->running.load(); }

void network::close(net_lock const &lck) { this->running.store(false); }

std::weak_ptr<boost::asio::io_context> network::get_io()
{
    return std::weak_ptr(this->io);
}

void network::start_listening(std::uint16_t hostport_ipv4,
                              std::uint16_t hostport_ipv6, net_lock const &lck)
{
    try
    {
        // If the network listener is null, create it
        if (!this->net_listener)
        {
            this->net_listener = std::make_unique<listener>(
                hostport_ipv4, hostport_ipv6,
                std::bind(
                    [](tcp::socket new_sock, std::weak_ptr<network> net_ptr,
                       std::shared_ptr<boost::asio::io_context> io_context) {
                        if (auto net = net_ptr.lock())
                        {
                            auto net_lock = net->get_guard();
                            net->add_connection(
                                net_con::new_connection(std::move(new_sock),
                                                        io_context,
                                                        net->get_next_id()),
                                net_lock);
                        }
                    },
                    std::placeholders::_1, this->self, this->io),
                this->io);
            if (!net_listener->is_open()) {}
        }
    }
    catch (std::exception &e)
    {
    }
}

con_id network::get_next_id()
{
    con_id new_con_id = 0;
    do
    {
        new_con_id = this->next_id.load();
    } while (!this->next_id.compare_exchange_weak(new_con_id, new_con_id + 1));

    return new_con_id;
}

void network::set_new_msg_callback(
    std::function<void(con_id, reader_ptr)> callback)
{
    this->new_msg_callback = callback;
}

void network::set_new_connection_callback(
    std::function<void(net_con &)> callback)
{
    this->new_connection_callback = callback;
}

void network::set_closed_connection_callback(
    std::function<void(net_con &)> callback)
{
    this->closed_connection_callback = callback;
}

void network::process_new_msgs() { this->received_msgs.notify_listeners(); }

std::weak_ptr<network> network::get_net_weak() { return this->self; }
