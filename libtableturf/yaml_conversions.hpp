#pragma once

#include "yaml-cpp/yaml.h"
#include "card.hpp"

namespace YAML {
    template<>
    struct convert<Card> {
        static Node encode(const Card& card);
        static bool decode(const Node& node, Card& rhs);
    };
}
