#include "tableturf.hpp"
#include <vector>
#include <stdexcept>

#define BOOST_TEST_MODULE board_layouts

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test) {
    // Valid boards
    std::pair<int, int> p1_start = {0, 0};
    std::pair<int, int> p2_start = {1, 1};
    std::vector<std::pair<int, int>> coords = { {0, 0}, {0, 1}, {1, 0}, {1, 1}};

    BoardLayout b1(coords, p1_start, p2_start);

    BOOST_TEST(b1.tiles.contains({0, 0}), "board doesnt contain correct coordinates");
    BOOST_TEST(b1.tiles.contains({1, 1}), "board doesnt contain correct coordinates");

    BOOST_TEST(p1_start.first == b1.p1_start.first); BOOST_TEST(p1_start.second == b1.p1_start.second);
    BOOST_TEST(p2_start.first == b1.p2_start.first); BOOST_TEST(p2_start.second == b1.p2_start.second);
    
    // Invalid boards
    // the e stands for evil
    try {
        // empty board layout
        BoardLayout eb1(std::vector<std::pair<int, int>>({}), p1_start, p2_start);
        BOOST_TEST(false, "constructor accepted empty board layout");
    } catch (std::runtime_error& e) {}

    try {
        // start square not on the board
        BoardLayout eb1(coords, {7, 27}, p2_start);
        BOOST_TEST(false, "constructor accepted board layout with invalid start square");
    } catch (std::runtime_error& e) {}

    try {
        // p1 and p2 start are the same
        BoardLayout eb1(coords, p2_start, p2_start);
        BOOST_TEST(false, "constructor accepted board where both players start on the same square");
    } catch (std::runtime_error& e) {}

}
