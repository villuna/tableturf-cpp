#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

enum Player {
    PLAYER_P1,
    PLAYER_P2,
};

struct Tile {
    Player owner;
    bool is_special;
};

// A struct that describes a game board that can be played on.
//
// This struct is not meant to record the state of a game in progress, rather it describes the
// layout of a board.
struct BoardLayout {
    // The upper bound dimensions of the board.
    // This is the width and height of the bounding box containing the board. The board might
    // not have all of these tiles filled, but we know that any tile will have coordinates that
    // are less than these values.
    int width;
    int height;

    // The special tiles where p1 and p2 start
    std::pair<int, int> p1_start;
    std::pair<int, int> p2_start;

    std::map<std::pair<int, int>, std::optional<Tile>> tiles;

    // This really shouldn't be a thing (uninitialised and bad) but like... c++ likes it when you
    // define an empty constructor
    BoardLayout() {}
    BoardLayout(
        const std::vector<std::pair<int, int>>& coordinates, 
        std::pair<int, int> p1_start, 
        std::pair<int, int> p2_start
    );
    BoardLayout(const std::string& desc);
};

// The state of a board which is currently in play
class Board {
    BoardLayout layout;

public:
    Board() {}
    Board(BoardLayout layout);
};
