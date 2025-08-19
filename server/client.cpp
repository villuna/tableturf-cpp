#include "protocol.hpp"
#include "server.hpp"
#include "client.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

namespace asio = boost::asio;

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
        boost::bind(&ClientConnection::handle_recv, 
            shared_from_this(),
            asio::placeholders::error,
            asio::placeholders::bytes_transferred
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
            boost::bind(&ClientConnection::handle_send, 
                shared_from_this(),
                asio::placeholders::error
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

    ClientMessage c;
    try {
        c = client_message_from_json(line);
    } catch (std::exception& e) {
        std::cerr << "Error parsing client message: " << e.what() << std::endl;
        disconnect();
        return;
    }

    auto response = m_server.handle_client_message(m_id, c);

    if (response.has_value()) {
        queue_send_message(*response);
    }

    // Get ready to recieve the next client message 
    start_recv_message();
}

void ClientConnection::disconnect() {
    socket.close();
}
