#pragma once

#include <map>
#include <optional>
#include <string>
#include <tuple>

enum PlayerSide {
    PLAYER_NONE,
    PLAYER_P1,
    PLAYER_P2,
};

struct Tile {
    bool is_wall;

    // These two have no meaning if the struct is a wall
    PlayerSide owner;
    bool is_special;

    Tile() :
        is_wall(true),
        owner(PLAYER_NONE),
        is_special(true)
    {}

    Tile(PlayerSide owner, bool is_special) :
        is_wall(false),
        owner(owner),
        is_special(is_special)
    {}

    inline friend bool operator==(const Tile& lhs, const Tile& rhs) { 
        return std::tie(lhs.is_wall, lhs.owner, lhs.is_special) == std::tie(rhs.is_wall, rhs.owner, rhs.is_special);
    }
    inline friend bool operator!=(const Tile& lhs, const Tile& rhs) { return !(lhs == rhs); }
};

struct Coord {
    int x;
    int y;

public:
    Coord() : x(0), y(0) {}
    Coord(int x, int y) : x(x), y(y) {}

    inline friend bool operator<(const Coord& lhs, const Coord& rhs) { 
        return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
    }
    inline friend bool operator>(const Coord& lhs, const Coord& rhs) { return rhs < lhs; }
    inline friend bool operator<=(const Coord& lhs, const Coord& rhs) { return !(lhs > rhs); }
    inline friend bool operator>=(const Coord& lhs, const Coord& rhs) { return !(lhs < rhs); } 
};

typedef std::map<Coord, std::optional<Tile>> BoardState;

// Parses a board state in FIN encoding.
// FIN, or Fishyth-Ika notation, is an encoding scheme devised by Tacchios Fishyth and Veron Ika
// for the purpose of transmitting tableturf board states between programs.
//
// The board layout is encoded in lines, somewhat similar to the FEN chess notation widely used in
// human society. Each line, starting from the top down, is described by a list of characters
// representing squares. The lines are separated by the character '/'.
//
// The different square types are:
//
// - 'f': a free square, where something could be placed but is empty.
// - 'w': a wall, which visually looks like a tile but where nothing can be placed.
// - 'x': an empty square, where nothing can be placed.
// - 'a'/'A': a tile belonging to player 1. Capitals denote special squares.
// - 'b'/'B': a tile belonging to player 2. "        "      "       "
//
// A single character represents one square, and a number followed by a character represents a
// number of squares. E.g., 'A' means one special tile belonging to P1, and '5A' means 5 of them.
//
// E.g: this board:
//
// +---+---+---+---+
// |   |   | b | B |
// +---+---+---+---+
// |   |   |   |   |
// +---+---+---+---+
// |   |   |   |   |
// +---+---+---+---+
// | A | a |   |   |
// +---+---+---+---+
//
// Is represented as
//
// 2fbB/4f/4f/Aa2f
//
// Any lines that are shorter than the maximum width are considered to have spaces inserted at the end. So this board:
//
//         +---+---+---+   
//         |   | A |   |
// +---+---+---+---+---+
// |   |   |   |   |   |
// +---+---+---+---+---+
// |   |   |   |   |   |
// +---+---+---+---+---+
// |   | B |   |
// +---+---+---+   
//
// Can be represented as
//
// 2xfAf/5f/5f/fBf2x
//
// Or as
//
// 2xfAf/5f/5f/fBf
//
// The last line appears to have a width of 3, and the spaces are inserted into the line afterwards.
// Note that the first line still needs its spaces at the beginning.
BoardState parse_fin_state(std::string fin_str);

// A board for playing tableturf on.
class Board {
    // The upper bound dimensions of the board.
    // This is the width and height of the bounding box containing the board. The board might
    // not have all of these tiles filled, but we know that any tile will have coordinates that
    // are less than these values.
    int width, height;
    // The name of the board layout
    std::string name;
    // The state of the board
    BoardState state;

public:
    // Create a board from an initial board state.
    // Throws an exception if it is an invalid starting state.
    Board(std::string name, BoardState initial_state);
    Board(std::string name, const std::string& fin_state);

    const std::string& get_name() const { return name; }
    const int get_width() const { return width; }
    const int get_height() const { return height; }
    const BoardState& get_board_state() const { return state; }

    // Generate a fin representation of the board
    std::string to_fin_str() const;
};
