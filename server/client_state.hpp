#pragma once

#include <optional>
#include "protocol.hpp"
#include "server.hpp"

// Virtual class representing the state of a client connection. Doesn't do any of the asynchronous
// IO logic (that is handled in the ClientConnection class and the messages are simply passed into
// this object), but instead handles messages and other game state.
class ClientState {
protected:
    Server& m_server;
    ClientState(Server& server): m_server(server) {}

public:
    // Handles a client message. Returns an (optional) message to respond with and an (optional) new
    // client state to switch to.
    virtual std::optional<std::unique_ptr<ClientState>> handle_message(
        ClientConnection& connection,
        ClientMessage msg
    ) { return std::nullopt; }
};

// A state that simply waits for the client to send HelloServer
class GreetingState : public ClientState {
public:
    std::optional<std::unique_ptr<ClientState>> handle_message(
        ClientConnection& connection,
        ClientMessage msg
    ) override;

    GreetingState(Server& server) : ClientState(server) {}
};

class LobbyState : public ClientState {
private:
    PublicPlayerInfo m_player_info;

public:
    std::optional<std::unique_ptr<ClientState>> handle_message(
        ClientConnection& connection,
        ClientMessage msg
    ) override;

    LobbyState(Server& server, PublicPlayerInfo info) : ClientState(server), m_player_info(info) {}
};
