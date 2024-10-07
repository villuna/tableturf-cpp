#include "board.hpp"
#include <cassert>
#include <optional>
#include <stdexcept>

BoardLayout::BoardLayout(const std::vector<std::pair<int, int>>& coordinates, std::pair<int, int> p1_start, std::pair<int, int> p2_start) :
    p1_start(p1_start),
    p2_start(p2_start)
{
    if (p1_start == p2_start) {
        throw std::runtime_error("Invalid board: p1 and p2 can't have the same start tile");
    }

    bool p1_start_valid = false;
    bool p2_start_valid = false;

    if (coordinates.empty()) {
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
    for (auto i = coordinates.begin(); i != coordinates.end(); i++) {
        if (i->first < 0 || i->second < 0) {
            throw std::runtime_error("Invalid board: coordinates cannot be negative");
        }

        // Figure out the bounding box of the board by finding what the min and max x and y values are
        if (!min_x || i->first < *min_x)
            min_x = i->first;
        if (!max_x || i->first > *max_x)
            max_x = i->first;

        if (!min_y || i->second < *min_y)
            min_y = i->second;
        if (!max_y || i->second> *max_y)
            max_y = i->second;

        // Ensure that the p1 and p2 starts are actually in the list of coordinates
        if (*i == p1_start)
            p1_start_valid = true;
        if (*i == p2_start)
            p2_start_valid = true;
    }

    if (!p1_start_valid || !p2_start_valid) {
        throw std::runtime_error("Invalid board: players' start squares must be in valid coordinates");
    }

    assert(min_x && min_y && max_x && max_y);

    width = *max_x - *min_x;
    height = *max_y - *min_y;

    for (auto i = coordinates.begin(); i != coordinates.end(); i++) {
        // normalise coordinates
        std::pair coord = *i;
        coord.first -= *min_x;
        coord.second -= *min_y;

        // If the insertion failed (i.e., we have inserted the same coordinate twice), that's invalid.
        // We could also ignore it, but I'm choosing to be a hardass.
        if (!tiles.insert({ coord, std::nullopt }).second) {
            throw std::runtime_error("Invalid board: coordinates must all be unique");
        }
    }
}
