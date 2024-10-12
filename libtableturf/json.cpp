#include "json.hpp"
#include "board.hpp"

void from_json(const json& j, Coord& c) {
    j.at(0).get_to(c.x);
    j.at(1).get_to(c.y);
}

void to_json(json& j, const Coord& c) {
    j = json::array({c.x, c.y});
}
