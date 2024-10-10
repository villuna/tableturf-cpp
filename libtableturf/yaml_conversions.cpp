#include "yaml_conversions.hpp"

namespace YAML {
    Node convert<Card>::encode(const Card& card) {
        Node node;
        node["name"] = card.name;
        node["id"] = card.id;
        node["cost"] = card.special_cost;

        std::vector<Node> spaces;

        for (auto tile = card.tiles.begin(); tile != card.tiles.end(); tile++) {
            Node t;
            t["coord"] = Node();
            t["coord"].push_back(tile->first.x);
            t["coord"].push_back(tile->first.y);

            t["special"] = tile->second;

            spaces.push_back(t);
        }

        node["spaces"] = spaces;

        return node;
    }

    bool convert<Card>::decode(const Node& node, Card& rhs) {
        // Check all the required fields are provided
        if (!(node["name"] && node["id"] && node["cost"] && node["spaces"])) {
            return false;
        }

        // Try reading from the node and return false if any of them fail to match
        try {
            Card card {};

            card.name = node["name"].as<std::string>();
            card.id = static_cast<CardId>(node["id"].as<int>());
            card.special_cost = node["cost"].as<int>();

            std::vector<Node> squares = node["spaces"].as<std::vector<Node>>();

            for (auto square : squares) {
                std::pair<Coord, bool> tile;

                if (!(square["coord"] && square["special"] && square["coord"].IsSequence() && square["coord"].size() == 2)) {
                    return false;
                }

                tile.first.x = square["coord"][0].as<int>();
                tile.first.y = square["coord"][1].as<int>();
                tile.second = square["special"].as<bool>();

                card.tiles.push_back(tile);
            }

            rhs = card;
            return true;
        } catch (InvalidNode& _) {
            return false;
        };
    }
}
