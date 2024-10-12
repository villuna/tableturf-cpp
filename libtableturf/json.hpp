#pragma once

#include <nlohmann/json.hpp>
#include "card.hpp"

using json = nlohmann::json;

void to_json(json& j, const Coord& c);
void from_json(const json& j, Coord& c);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CardTile, coord, special);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Card, name, id, special_cost, tiles);
