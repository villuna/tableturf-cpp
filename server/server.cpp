#include "server.hpp"
#include "protocol.hpp"
#include <boost/asio.hpp>
#include <boost/asio/impl/read.hpp>
#include <boost/bind/bind.hpp>
#include <exception>
#include <iostream>
#include <string>
#include <variant>

Server::Server(asio::io_context& ctx) :
    m_ctx(ctx),
    m_acceptor(ctx, tcp::endpoint(tcp::v4(), 2611))
{
    accept_connection();
}

void Server::accept_connection() {
    auto client = ClientConnection::make(m_ctx);

    m_acceptor.async_accept(client->socket, 
            boost::bind(&Server::handle_accept, this, asio::placeholders::error, client));
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

ClientConnection::~ClientConnection() {
    std::cout << "Destroying connection to client" << std::endl;
}

void ClientConnection::start() {
    start_recv_message();
}

void ClientConnection::queue_send_message(ServerMessage msg) {
    m_write_queue.push(msg);

    if (!m_sending) {
        m_sending = true;
        send_next_message();
    }
}

void ClientConnection::start_recv_message() {
    asio::async_read_until(
        socket,
        asio::dynamic_buffer(m_read_buffer),
        '\n',
        asio::bind_executor(m_strand,
            boost::bind(&ClientConnection::handle_recv, 
                shared_from_this(),
                asio::placeholders::error,
                asio::placeholders::bytes_transferred
            )
        )
    );
}

void ClientConnection::handle_send(const error_code& e) {
    if (e) {
        std::cerr << "Error sending message: " << e.what() << std::endl;
        disconnect();
    } else if (!m_write_queue.empty()) {
        send_next_message();
    } else {
        m_sending = false;
    }
}

void ClientConnection::send_next_message() {
    if (!m_write_queue.empty()) {
        ServerMessage msg = m_write_queue.front();
        m_write_queue.pop();

        m_write_buffer = server_message_to_json(msg);
        m_write_buffer.push_back('\n');

        asio::async_write(
            socket,
            asio::buffer(m_write_buffer),
            asio::bind_executor(m_strand,
                boost::bind(&ClientConnection::handle_send, 
                    shared_from_this(),
                    asio::placeholders::error
                )
            )
        );
    }
}

void ClientConnection::handle_recv(const error_code& e, size_t bytes_transferred) {
    if (e) {
        std::cerr << "Error recieving client message: " << e.what() << std::endl;
        disconnect();
        return;
    }

    std::string line = m_read_buffer.substr(0, bytes_transferred);
    std::cout << "Recieved line: " << line << std::endl;
    m_read_buffer.erase(0, bytes_transferred);

    try {
        ClientMessage c = client_message_from_json(line);
        handle_client_message(c);
    } catch (std::exception& e) {
        std::cerr << "Error parsing client message: " << e.what() << std::endl;
    }

    // Get ready to recieve the next client message 
    start_recv_message();
}

void ClientConnection::handle_client_message(ClientMessage msg) {
    if (std::holds_alternative<HelloServer>(msg)) {
        // TODO: Server stuff
        HelloServer hello = std::get<HelloServer>(msg);
        std::cout << "Player \"" << hello.info.name << "\" joined" << std::endl;

        queue_send_message(HelloClient {});
    }
}

void ClientConnection::disconnect() {
    socket.close();
}
