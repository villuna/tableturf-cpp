#pragma once

#include <string>
#include <array>
#include "board.hpp"
#include "card.hpp"

// Describes the state of a player in the match.
class PlayerState {
    std::string name;
    std::array<CardId, 15> deck;

public:
    PlayerState(std::string name) : name(name) {}
};

// Describes the state of a match being played.
class Match {
    Board board;
    PlayerState p1, p2;
    int turns_left;

public:
    Match(Board initial_board, PlayerState p1, PlayerState p2) :
        board(initial_board),
        p1(p1),
        p2(p2),
        turns_left(15)
    {}
};
