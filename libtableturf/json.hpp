#pragma once

#include <nlohmann/json.hpp>
#include "card.hpp"

using json = nlohmann::json;

void to_json(json& j, const Card& c);
void from_json(const json& j, Card& c);
