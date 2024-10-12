#define BOOST_TEST_MODULE card_json

#include <boost/test/included/unit_test.hpp>

#include "card.hpp"
#include "json.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;    

BOOST_AUTO_TEST_CASE(json_decode) {
    std::string spshjr = R"({
        "name": "Splattershot Jr.",
        "id": 3,
        "cost": 2,
        "tiles": [
            { "coord": [2, 3], "special": false },
            { "coord": [3, 3], "special": true },
            { "coord": [4, 3], "special": false },
            { "coord": [2, 4], "special": false }
        ]
    })";

    json spshjr_json = json::parse(spshjr);
    std::cout << spshjr_json.dump() << std::endl;
    Card spshjr_card = spshjr_json;

    BOOST_TEST(spshjr_card.name == "Splattershot Jr.");
    BOOST_TEST(spshjr_card.id == 3);
    BOOST_TEST(spshjr_card.special_cost == 2);
    BOOST_TEST(spshjr_card.tiles.size() == 4);
}

BOOST_AUTO_TEST_CASE(json_test) {
    json j;
    j["test"].push_back(1);
    BOOST_TEST(j.dump() == R"({"test":[1]})");
}
