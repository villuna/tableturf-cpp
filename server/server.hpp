#pragma once

#include <protocol.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <queue>

namespace asio = boost::asio;
using asio::ip::tcp;
using boost::system::error_code;

// A class that represents a connection to a game client.
// This class manages the socket used to communicate with that client, and schedules all the
// async IO events used to communicate with it.
//
// This connection is always used through a shared_ptr so that its memory can be kept alive even
// though it is being passed from callback to callback.
//
// This model is the same as in the Boost.Asio async tcp server tutorial.
class ClientConnection : public std::enable_shared_from_this<ClientConnection> {
    asio::strand<asio::io_context::executor_type> m_strand;

    asio::streambuf m_read_buffer;

    std::string m_write_buffer;
    std::queue<std::string> m_write_queue;

    ClientConnection(asio::io_context& ctx) : m_strand(asio::make_strand(ctx)), socket(ctx) {}

    void queue_send_message(ServerMessage msg);
    void send_message(std::string message);

    void start_recv_message();

    // Handlers that are called upon completion of an async send/recv
    void handle_send(const error_code& e);
    void handle_recv(const error_code& e, size_t bytes_transferred);

    // Handles a (parsed) client message.
    void handle_client_message(ClientMessage msg);
public:
    tcp::socket socket;

    ~ClientConnection();

    static std::shared_ptr<ClientConnection> make(asio::io_context& ctx) {
        return std::shared_ptr<ClientConnection>(new ClientConnection(ctx));
    }

    void start();
};

class Server {
    asio::io_context& m_ctx;
    tcp::acceptor m_acceptor; 

    void accept_connection();
    void handle_accept(const error_code& e, std::shared_ptr<ClientConnection> connection);

public:
    Server(asio::io_context& ctx);
};

