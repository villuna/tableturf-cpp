#include "client.hpp"
#include <atomic>
#include <boost/asio/connect.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>

bool Client::connect() {
    bool expected = false;
    m_online.compare_exchange_strong(expected, true,
            std::memory_order::memory_order_acquire,
            std::memory_order_relaxed);

    if (!expected) {
        // We were not online and have now gone online.
        // We issue a connection.
        std::cout << "Issuing async resolve" << std::endl;
        m_resolver.async_resolve("127.0.0.1", "2611",
                boost::bind(&Client::handle_resolve, this, asio::placeholders::error, asio::placeholders::results));
    }

    return expected;
}

void Client::handle_resolve(const error_code& e, tcp::resolver::results_type results) {
    std::cout << "Resolve has finished" << std::endl;
    if (e) {
        std::cerr << "Error while resolving server address: " << e.what() << std::endl;
        m_online.store(false, std::memory_order_release);
        return;
    }

    asio::async_connect(m_socket, results,
            boost::bind(&Client::handle_connect, this, asio::placeholders::error, asio::placeholders::endpoint));
}

void Client::handle_connect(const error_code& e, const tcp::endpoint& endpoint) {
    std::cout << "Connect has finished" << std::endl;
    if (e) {
        std::cerr << "Error while connecting to server: " << e.what() << std::endl;
        m_online.store(false, std::memory_order_release);
        return;
    }

    asio::async_read_until(m_socket, m_read_buffer, '\n', 
            boost::bind(&Client::handle_read, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

void Client::handle_read(const error_code& e, size_t bytes_read) {
    std::cout << "Read has finished" << std::endl;
    if (e) {
        std::cerr << "Error while reading from server: " << e.what() << std::endl;
        m_socket.close();
        m_online.store(false, std::memory_order_release);
        return;
    }

    std::stringstream strstrm;
    strstrm << &m_read_buffer;

    std::string line;
    // This will let us get rid of the newline
    std::getline(strstrm, line);
    std::cout << "Recieved line: " << line << std::endl;

    try {
        ServerMessage msg = server_message_from_json(line);
        m_tx.send(msg);
    } catch (std::exception& e) {
        std::cerr << "Error parsing server message: " << e.what() << std::endl;
    }

    asio::async_read_until(m_socket, m_read_buffer, '\n', 
            boost::bind(&Client::handle_read, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

void Client::send_message(ClientMessage msg) {
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

void Client::handle_send(const error_code& e) {
    if (e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}
