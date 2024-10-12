#pragma once
#include <cstdint>
#include <vector>
#include "board.hpp"

// This should be enough card ids I think
typedef uint32_t CardId;

struct CardTile {
    Coord coord;
    bool special;
};

struct Card {
    CardId id;

    // A list containing the positions of all the tiles in this card (relative to the centre of
    // the card pattern), and whether or not they are special tiles
    std::vector<CardTile> tiles;
    // How many special points it costs to play this card as a special attack
    int special_cost;

    std::string name;
    // The path to the file (in the client's resources directory) containing the card's face image
    std::string image_file;

public:
    Card() {}
};

std::map<CardId, Card> read_card_map(const std::string& filename);
