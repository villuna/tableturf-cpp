#include "client.hpp"
#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>

void Client::connect() {
    if (!m_online) {
        std::cout << "Issuing async resolve" << std::endl;
        m_resolver.async_resolve("127.0.0.1", "2611",
                boost::bind(&Client::handle_resolve, this, asio::placeholders::error, asio::placeholders::results));

        m_online = true;
    }
}

void Client::disconnect() {
    if (m_online) {
        m_resolver.cancel();
        m_socket.close();

        // Since we will be reusing all our stuff if we connect again, we'd better reset everything
        m_out_messages.clear();
        m_sending = false;
        m_read_buffer.clear();
        m_write_buffer.clear();
        m_online = false;
        m_connected = false;
    }
}

void Client::handle_resolve(const error_code& e, tcp::resolver::results_type results) {
    std::cout << "Resolve has finished" << std::endl;
    if (e) {
        std::cerr << "Error while resolving server address: " << e.what() << std::endl;
        return;
    }

    asio::async_connect(m_socket, results,
            boost::bind(&Client::handle_connect, this, asio::placeholders::error, asio::placeholders::endpoint));
}

void Client::handle_connect(const error_code& e, const tcp::endpoint& endpoint) {
    std::cout << "Connect has finished" << std::endl;
    if (e) {
        std::cerr << "Error while connecting to server: " << e.what() << std::endl;
        disconnect();
    } else {
        m_connected = true;

        asio::async_read_until(m_socket, asio::dynamic_buffer(m_read_buffer), '\n', 
                boost::bind(&Client::handle_read, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
    }
}

void Client::handle_read(const error_code& e, size_t bytes_read) {
    std::cout << "Read has finished" << std::endl;
    if (e) {
        std::cerr << "Error while reading from server: " << e.what() << std::endl;
        disconnect();

        return;
    }

    std::string line = m_read_buffer.substr(0, bytes_read);
    std::cout << "Recieved line: " << line << std::endl;

    m_read_buffer.erase(0, bytes_read);

    try {
        ServerMessage msg = server_message_from_json(line);
        m_in_messages.push_back(msg);
    } catch (std::exception& e) {
        std::cerr << "Error parsing server message: " << e.what() << std::endl;
    }

    asio::async_read_until(m_socket, asio::dynamic_buffer(m_read_buffer), '\n', 
            boost::bind(&Client::handle_read, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

void Client::queue_send(ClientMessage msg) {
    if (m_online) {
        m_out_messages.push_back(msg);

        if (!m_sending) {
            m_sending = true;
            send_next_message();
        }
    }
}

void Client::send_next_message() {
    if (!m_out_messages.empty()) {
        ClientMessage msg = m_out_messages.front();
        m_out_messages.pop_front();

        std::string message = client_message_to_json(msg);
        m_write_buffer = message;
        m_write_buffer.push_back('\n');

        std::cout << "Sending message " << message << std::endl;

        asio::async_write(
            m_socket,
            asio::buffer(m_write_buffer),
            boost::bind(&Client::handle_send, 
                this,
                asio::placeholders::error
            )
        );
    }
}

void Client::handle_send(const error_code& e) {
    if (e) {
        std::cerr << "Error sending message to server: " << e.what() << std::endl;
        disconnect();
        return;
    } else if (!m_out_messages.empty()) {
        send_next_message();
    } else {
        m_sending = false;
    }
}
