#define BOOST_TEST_MODULE card_json

#include <boost/test/included/unit_test.hpp>
#include <nlohmann/json.hpp>
#include "protocol.hpp"
#include <variant>

BOOST_AUTO_TEST_CASE(client_message) {
    ClientMessage m = HelloServer { .info = PublicPlayerInfo { .name = "villuna" } };
    std::cout << __LINE__ << std::endl;
    std::string json = client_message_to_json(m);
    std::cout << __LINE__ << std::endl;
    std::string expected = "{\"info\":{\"name\":\"villuna\"},\"type\":\"" + std::to_string(CL_HELLO_SERVER) + "\"}";

    BOOST_TEST(json == expected);

    ClientMessage m2 = client_message_from_json(json);
    std::cout << __LINE__ << std::endl;
    BOOST_TEST(std::holds_alternative<HelloServer>(m2));
    BOOST_TEST(std::get<HelloServer>(m2).info.name == std::get<HelloServer>(m).info.name);
}

BOOST_AUTO_TEST_CASE(server_message) {
    ServerMessage m = HelloClient {};
    std::string json = server_message_to_json(m);
    std::string expected = "{\"type\":\"" + std::to_string(CL_HELLO_SERVER) + "\"}";

    BOOST_TEST(json == expected);

    ServerMessage m2 = server_message_from_json(json);
    BOOST_TEST(std::holds_alternative<HelloClient>(m2));
}
