#include "board.hpp"
#include <stdexcept>

#define BOOST_TEST_MODULE board_layouts

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_boards) {
    // Valid boards
    BoardState state = {
        {{0, 0}, Tile(PLAYER_P1, true)},
        {{0, 1}, std::nullopt },
        {{1, 0}, std::nullopt },
        {{1, 1}, Tile(PLAYER_P2, true)},
    };

    Board b1("Four Square", state);

    BOOST_TEST(b1.get_board_state().contains({0, 0}), "board doesnt contain correct coordinates");
    BOOST_TEST(b1.get_board_state().contains({1, 1}), "board doesnt contain correct coordinates");

    std::optional<Tile> p1_tile = b1.get_board_state().at({0, 0});

    BOOST_TEST(p1_tile.has_value());
    BOOST_TEST(p1_tile.value().owner == PLAYER_P1);
    BOOST_TEST(p1_tile.value().is_special);
    
    // Invalid boards
    try {
        // empty board layout
        Board eb("Empty", BoardState{});
        BOOST_TEST(false, "constructor accepted empty board layout");
    } catch (std::runtime_error& e) {}

    try {
        // only p1 has a start square
        BoardState p1_wins(state);
        p1_wins.erase({1, 1});
        
        Board p1wb("Player 1 Wins", p1_wins);
        BOOST_TEST(false, "constructor accepted board where only one player has a start square");
    } catch (std::runtime_error& e) {}

}
