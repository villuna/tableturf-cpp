#pragma once

#include "client_state.hpp"
#include "server.hpp"
#include <queue>

// A class that represents a connection to a game client.
// This class manages the socket used to communicate with that client, and schedules all the
// async IO events used to communicate with it.
//
// This connection is always used through a shared_ptr so that its memory can be kept alive even
// though it is being passed from callback to callback.
//
// This model is the same as in the Boost.Asio async tcp server tutorial.
class ClientConnection : public std::enable_shared_from_this<ClientConnection> {
    Server& m_server;
    ClientId m_id;
    std::unique_ptr<ClientState> m_state;

    std::string m_read_buffer;
    std::string m_write_buffer;
    std::queue<ServerMessage> m_write_queue;
    bool m_sending { false };

    ClientConnection(asio::io_context& ctx, Server& server, ClientId id) : m_server(server), m_id(id), m_state { std::make_unique<GreetingState>(server) }, socket(ctx) {}

    void send_next_message();

    void start_recv_message();

    // Handlers that are called upon completion of an async send/recv
    void handle_send(const error_code& e);
    void handle_recv(const error_code& e, size_t bytes_transferred);

    // Stop all correspondance with the client.
    void disconnect();
public:
    tcp::socket socket;

    ~ClientConnection();

    static std::shared_ptr<ClientConnection> make(asio::io_context& ctx, Server& server, ClientId id) {
        return std::shared_ptr<ClientConnection>(new ClientConnection(ctx, server, id));
    }

    void start();
    void queue_send_message(ServerMessage msg);
};
