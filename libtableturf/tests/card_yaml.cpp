#include "yaml-cpp/exceptions.h"
#define BOOST_TEST_MODULE card_yaml

#include <boost/test/included/unit_test.hpp>

#include "card.hpp"
#include "yaml_conversions.hpp"
#include "yaml-cpp/yaml.h"

BOOST_AUTO_TEST_CASE(yaml_decode) {
    std::string spshjr = R"(name: Splattershot Jr.
id: 3
cost: 2
spaces: [
  {coord: [2, 3], special: false},
  {coord: [3, 3], special: true},
  {coord: [4, 3], special: false},
  {coord: [2, 4], special: false},
])";

    YAML::Node spshjr_node = YAML::Load(spshjr);
    Card spshjr_card = spshjr_node.as<Card>();

    BOOST_TEST(spshjr_card.name == "Splattershot Jr.");
    BOOST_TEST(spshjr_card.id == 3);
    BOOST_TEST(spshjr_card.special_cost == 2);
    BOOST_TEST(spshjr_card.tiles.size() == 4);
}
