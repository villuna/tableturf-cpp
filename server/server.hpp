#pragma once

#include <cstdint>
#include <optional>
#include <protocol.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <unordered_map>

namespace asio = boost::asio;
using asio::ip::tcp;
using boost::system::error_code;

using ClientId = uint64_t;

class ClientConnection;

class Server {
    asio::io_context& m_ctx;
    tcp::acceptor m_acceptor;

    ClientId m_next_id { 0 };

    std::unordered_map<ClientId, PublicPlayerInfo> m_players;
    std::optional<ClientId> m_hot_seat;

    void accept_connection();
    void handle_accept(const error_code& e, std::shared_ptr<ClientConnection> connection);

public:
    Server(asio::io_context& ctx);

    // The ClientConnections need to hold a valid reference to this server object so disable
    // copying, for safety.
    Server(const Server& other) = delete;
    Server& operator=(const Server& other) = delete;
};
