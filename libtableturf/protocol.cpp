#include "protocol.hpp"
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include "json.hpp"

// Evil macro magic to avoid having to type the same thing over and over
#define MSG_PARSE_JSON(json_obj, tag, type) if (json_obj.contains("type") && json_obj["type"] == std::to_string(tag)) { \
    return json_obj.template get<type>(); }
#define MSG_PARSE_EMPTY_JSON(json_obj, tag, type) if (json_obj.contains("type") && json_obj["type"] == std::to_string(tag)) { \
    return type {}; }

#define MSG_SERIALISE_JSON(msg, tag, type) if (std::holds_alternative<type>(msg)) { \
    nlohmann::json j = std::get<type>(msg); \
    j["type"] = std::to_string(tag); \
    return j.dump(); }

#define MSG_SERIALISE_EMPTY_JSON(msg, tag, type) if (std::holds_alternative<type>(msg)) { \
    nlohmann::json j {}; \
    j["type"] = std::to_string(tag); \
    return j.dump(); }


ClientMessage client_message_from_json(std::string json_str) {
    using json = nlohmann::json;
    json j = json::parse(json_str);

    MSG_PARSE_JSON(j, CL_HELLO_SERVER, HelloServer);

    throw std::invalid_argument("Could not parse json string to client message - not a valid variant");
}

std::string client_message_to_json(const ClientMessage& message) {
    MSG_SERIALISE_JSON(message, CL_HELLO_SERVER, HelloServer);

    throw std::invalid_argument("message was not an accepted variant");
}

ServerMessage server_message_from_json(std::string json_str) {
    using json = nlohmann::json;
    json j = json::parse(json_str);

    MSG_PARSE_EMPTY_JSON(j, SV_HELLO_CLIENT, HelloClient);

    throw std::invalid_argument("Could not parse json string to client message - not a valid variant");
}

std::string server_message_to_json(const ServerMessage& message) {
    MSG_SERIALISE_EMPTY_JSON(message, SV_HELLO_CLIENT, HelloClient);

    throw std::invalid_argument("message was not an accepted variant");
}
