#include "server.hpp"
#include "client.hpp"
#include "protocol.hpp"
#include <boost/asio.hpp>
#include <boost/asio/impl/read.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <optional>
#include <variant>

Server::Server(asio::io_context& ctx) :
    m_ctx(ctx),
    m_acceptor(ctx, tcp::endpoint(tcp::v4(), 2611))
{
    accept_connection();
}

void Server::accept_connection() {
    auto client = ClientConnection::make(m_ctx, *this, m_next_id++);

    m_acceptor.async_accept(client->socket, 
            boost::bind(&Server::handle_accept, this, asio::placeholders::error, client));
}

std::optional<ServerMessage> Server::handle_client_message(uint64_t id, ClientMessage msg) {
    if (std::holds_alternative<HelloServer>(msg)) {
        HelloServer& h = std::get<HelloServer>(msg);

        std::cout << "Client \"" << h.info.name << "\" said hello" << std::endl;

        return HelloClient {};
    }

    return std::nullopt;
}

void Server::handle_accept(const error_code& e, std::shared_ptr<ClientConnection> connection) {
    if (e) {
        std::cerr << "Encountered error: " << e.what() << std::endl;
    } else {
        connection->start();
    }

    // Even if we had an error with the previous connection, we'll keep trying to accept new clients
    accept_connection();
}
