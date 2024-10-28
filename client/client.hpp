#pragma once

// Handles a connection to the server and sends messages to the main thread via a channel.
#include "protocol.hpp"
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <deque>

namespace asio = boost::asio;
using asio::ip::tcp;
using boost::system::error_code;

class Client {
    tcp::resolver m_resolver;
    tcp::socket m_socket;

    // A queue of messages to send
    // Due to latency we might end up with more messages than we need
    std::deque<ClientMessage> m_out_messages;
    bool m_sending { false };

    // A queue of messages recieved from the server
    // This is shared with the main function. Since the async runtime is single threaded,
    // I don't think this needs to be protected with a mutex.
    std::deque<ServerMessage>& m_in_messages;

    // Buffers for storing the messages we send/recieve.
    // Async io happens over some time so we need to have some nice place to store en-route data
    std::string m_read_buffer;
    std::string m_write_buffer;

    // Indicates if the client is either trying to connect to the server or if it is already connected
    bool m_online;
    
    void handle_resolve(const error_code& e, tcp::resolver::results_type result);
    void handle_connect(const error_code& e, const tcp::endpoint& endpoint);
    void handle_read(const error_code& e, size_t bytes_read);
    void handle_send(const error_code& e);
    void send_next_message();

public:
    Client(std::deque<ServerMessage>& queue, asio::io_context& ctx) :
        m_resolver(ctx),
        m_socket(ctx),
        m_in_messages(queue),
        m_online(false)
    {}

    void connect();
    void disconnect();
    bool is_online() { return m_online; }
    // Queues a message to be send to the server.
    //
    // As implied by the name, the send isn't necessarily done straight away. If we are already
    // waiting for another message to be sent, we can't send another at the same time, so we store
    // the message in a queue.
    void queue_send(ClientMessage msg);
};
