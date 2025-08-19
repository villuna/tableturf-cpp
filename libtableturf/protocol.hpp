#pragma once

#include <string>
#include <variant>

// -- Client messages --

// HelloServer
struct PublicPlayerInfo {
    std::string name; 
};

struct HelloServer {
    PublicPlayerInfo info;
};

// FindGame
struct FindGame {};

typedef std::variant<
    HelloServer,
    FindGame
> ClientMessage;

// -- Server Messages --

// HelloClient
struct HelloClient {};

struct MatchFound {
    PublicPlayerInfo opp_info;
    int player_id;
};

typedef std::variant<
    HelloClient,
    MatchFound
> ServerMessage;

// Converts a json-encoded message string to a message variant
// throws an exception if the conversion failed
ClientMessage client_message_from_json(std::string json);
ServerMessage server_message_from_json(std::string json);

// Converts a message to a json-encoded byte string
std::string client_message_to_json(const ClientMessage& message);
std::string server_message_to_json(const ServerMessage& message);
