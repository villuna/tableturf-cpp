#include "card.hpp"
#include "yaml-cpp/yaml.h"
#include "yaml_conversions.hpp"
#include <map>
#include <print>



std::map<CardId, Card> read_card_map(const std::string& filename) {
    YAML::Node map = YAML::LoadFile(filename);
    std::vector<Card> cards = map.as<std::vector<Card>>();
    std::map<CardId, Card> res;

    for (auto c : cards) {
        res.insert({c.id, c});
    }

    return res;
}
