#pragma once

#include <string>
#include <variant>

// An enum which indicates the type of a client message. These tags will be used to externally tag
// the type of a message when encoded with json. If you want to remove one, try to mark its tag as
// deprecated so we don't reuse the same integer tag for different messages. This way if the server
// and client are different versions then there won't be weird mismatches.
//
// Is this way overengineered for this pet project? Of course. But writing good code is its own reward.
enum ClientMessageType {
    CL_HELLO_SERVER = 0,
};

enum ServerMessageType {
    SV_HELLO_CLIENT = 0,
};

// Message types: See their documentation in my notion notes.
// If for some reason you're reading this comment and you're not me, well. Maybe ask me to post them
// as markdown files.

// Client:

struct PublicPlayerInfo {
    std::string name; 
};

struct HelloServer {
    PublicPlayerInfo info;
};

typedef std::variant<
    HelloServer
> ClientMessage;

// Server:

struct HelloClient {};

typedef std::variant<
    HelloClient
> ServerMessage;

// Converts a json-encoded message string to a message variant
// throws an exception if the conversion failed
ClientMessage client_message_from_json(std::string json);
ServerMessage server_message_from_json(std::string json);

// Converts a message to a json-encoded byte string
std::string client_message_to_json(const ClientMessage& message);
std::string server_message_to_json(const ServerMessage& message);
