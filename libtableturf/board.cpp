#include "board.hpp"
#include <cassert>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <boost/algorithm/string.hpp>

Board::Board(std::string name, BoardState initial_state) :
    name(name),
    state()
{
    bool p1_has_tiles = false;
    bool p2_has_tiles = false;

    if (initial_state.empty()) {
        throw std::runtime_error("Invalid board: board cannot be empty");
    }

    std::optional<int> min_x = std::nullopt;
    std::optional<int> min_y = std::nullopt;
    std::optional<int> max_x = std::nullopt;
    std::optional<int> max_y = std::nullopt;

    // We do two passes, first to figure out the bounding box and validate the board, second to
    // actually insert the values. The reason we do this is so we can normalise the coordinates
    // i.e., we need to make sure that the min x and y values are both 0. This will help us with
    // actually dealing with coordinates later on.
    for (auto i = initial_state.begin(); i != initial_state.end(); i++) {
        std::pair<int, int> coord = i->first;
        std::optional<Tile> tile = i->second;

        if (coord.first < 0 || coord.second < 0) {
            throw std::runtime_error("Invalid board: coordinates cannot be negative");
        }

        // Figure out the bounding box of the board by finding what the min and max x and y values are
        if (!min_x || coord.first < *min_x)
            min_x = coord.first;
        if (!max_x || coord.first > *max_x)
            max_x = coord.first;

        if (!min_y || coord.second < *min_y)
            min_y = coord.second;
        if (!max_y || coord.second> *max_y)
            max_y = coord.second;

        // Ensure that the p1 and p2 both actually have somewhere to start
        if (tile) {
            if (tile->owner == PLAYER_P1) {
                p1_has_tiles = true;
            } else {
                p2_has_tiles = true;
            }
        }
    }

    if (!p1_has_tiles || !p2_has_tiles) {
        throw std::runtime_error("Invalid board: both players must have tiles on the board initially");
    }

    assert(min_x && min_y && max_x && max_y);

    width = *max_x + 1 - *min_x;
    height = *max_y + 1 - *min_y;

    for (auto i = initial_state.begin(); i != initial_state.end(); i++) {
        // normalise coordinates
        std::pair<int, int> coord = i->first;
        std::optional<Tile> tile = i->second;

        coord.first -= *min_x;
        coord.second -= *min_y;

        this->state.insert({ coord, tile });
    }
}

// TODO: Maybe replace this with a Boost.Spirit parser? But this works fine so not that necessary.
BoardState parse_fin_state(std::string fin_str) {
    // Trim space from the string just in case
    boost::trim(fin_str);

    std::stringstream sstream(fin_str);

    BoardState state {};
    std::string line;
    int y = 0;
    std::string num_string {};

    while (std::getline(sstream, line, '/')) {
        if (!num_string.empty()) {
            throw std::invalid_argument("FIN parsing error: number must be followed by a valid space character");
        }

        int x = 0;

        for (auto ci = line.begin(); ci != line.end(); ci++) {
            char c = *ci;

            if (c >= '0' && c <= '9') {
                if (num_string.empty() && c == '0') {
                    throw std::invalid_argument("FIN parsing error: numbers cannot start with 0");
                }

                num_string.push_back(c);
            } else {
                PlayerSide side;
                int num_squares = 1;

                if (!num_string.empty()) {
                    // If we have designated a number string
                    num_squares = std::stoi(num_string);
                    num_string.erase();
                }

                if (c == 'a' || c == 'A' || c == 'b' || c == 'B') {
                    if (c == 'a' || c == 'A')
                        side = PLAYER_P1;
                    else
                        side = PLAYER_P2;

                    bool is_special = c == 'A' || c == 'B';

                    Tile t = {
                        .owner = side,
                        .is_special = is_special,
                    };

                    for (int i = 0; i < num_squares; i++) {
                        state.insert({{x, y}, t});
                        x++;
                    }
                } else if (c == 'f') {
                    // Free spaces: insert them into the map but with no tile placed on them
                    for (int i = 0; i < num_squares; i++) {
                        state.insert({{x, y}, std::nullopt});
                        x++;
                    }
                } else if (c == 'x') {
                    // "Walls", empty spaces: don't insert them into the map
                    x += num_squares;
                } else {
                    throw std::invalid_argument("FIN parsing error: invalid character");
                }
            }
        }

        if (!num_string.empty()) {
            throw std::invalid_argument("FIN parsing error: number must be followed by a valid space character");
        }

        y++;
    }

    return state;
}
