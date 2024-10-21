#pragma once

// Handles a connection to the server and sends messages to the main thread via a channel.
#include "protocol.hpp"
#include "channel.hpp"
#include <atomic>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>

namespace asio = boost::asio;
using asio::ip::tcp;
using boost::system::error_code;

class Client {
    tcp::resolver m_resolver;
    tcp::socket m_socket;

    asio::streambuf m_read_buffer;
    std::string m_write_buffer;

    asio::steady_timer m_alive_check;

    // Indicates if the client is either trying to connect to the server or if it is already connected
    std::atomic<bool> m_online;
    Sender<ServerMessage> m_tx;

    void handle_resolve(const error_code& e, tcp::resolver::results_type result);
    void handle_connect(const error_code& e, const tcp::endpoint& endpoint);
    void handle_read(const error_code& e, size_t bytes_read);
    //void handle_timeout() {
    //    m_alive_check.expires_at(m_alive_check.expiry() + std::chrono::seconds(1));
    //    m_alive_check.async_wait(boost::bind(&Client::handle_timeout, this));
    //}

    void handle_send(const error_code& e);

public:
    Client(Sender<ServerMessage> tx, asio::io_context& ctx) :
        m_resolver(ctx),
        m_socket(ctx),
        m_alive_check(ctx, std::chrono::seconds(1)),
        m_online(false),
        m_tx(tx)
    {
        //m_alive_check.async_wait(boost::bind(&Client::handle_timeout, this));
    }

    ~Client() { std::cout << "Goodbye client" << std::endl; }

    // Tries to connect to the server. Returns a boolean indicating whether the client was already
    // connected or connecting.
    bool connect();
    bool is_online() { return m_online.load(); }
    void send_message(ClientMessage msg);
};
