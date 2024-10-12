#include "json.hpp"
#include "board.hpp"

void to_json(json& j, const Card& c) {
    // name id cost tiles
    j["name"] = c.name;
    j["id"] = c.id;
    j["cost"] = c.special_cost;

    json tiles;

    for (auto t : c.tiles) {
        json tile;
        tile["coord"].push_back(t.coord.x);
        tile["coord"].push_back(t.coord.y);
        tile["special"] = t.special;

        tiles.push_back(tile);
    }

    j["tiles"] = tiles;
}

void from_json(const json& j, Coord& c) {
    j.at(0).get_to(c.x);
    j.at(1).get_to(c.y);
}

void from_json(const json& j, CardTile& p) {
    j.at("coord").get_to(p.coord); 
    j.at("special").get_to(p.special);
}

void from_json(const json& j, Card& c) {
    j.at("name").get_to(c.name);
    j.at("id").get_to(c.id);
    j.at("cost").get_to(c.special_cost);
    j.at("tiles").get_to(c.tiles);
}
