#include "client_state.hpp"
#include "client.hpp"
#include <iostream>
#include <stdexcept>
#include <variant>

std::optional<std::unique_ptr<ClientState>> GreetingState::handle_message(
    ClientConnection& connection,
    ClientMessage msg
) {
    if (std::holds_alternative<HelloServer>(msg)) {
        auto m = std::get<HelloServer>(msg);
        std::cout << "Client \"" << m.info.name << "\" said hello" << std::endl;
        connection.queue_send_message(HelloClient {});
        return std::make_unique<LobbyState>(m_server, m.info);
    } else {
        throw new std::runtime_error("Invalid message");
    }
}

std::optional<std::unique_ptr<ClientState>> handle_message(
    ClientConnection& connection,
    ClientMessage msg
);

std::optional<std::unique_ptr<ClientState>> LobbyState::handle_message(
    ClientConnection& connection,
    ClientMessage msg
) {
    if (std::holds_alternative<FindGame>(msg)) {
        std::cout << "Find game" << std::endl;
    }

    return std::nullopt;
}
