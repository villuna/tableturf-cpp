#include "board.hpp"

#define BOOST_TEST_MODULE board_fin

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_fin_boards_valid) {
    std::string en_poisson = "8f/8f/5fB2f/8f/8f/2fA5f/8f/8f";
    std::string lakefront_property = "16f/16f/16f/12fB3f/16f/16f/6f4x6f/6f4x6f/6f4x6f/6f4x6f/16f/16f/3fA12f/16f/16f/16f";

    Board ep = Board("En Poisson", parse_fin_state(en_poisson));

    BOOST_TEST(ep.getWidth() == 8);
    BOOST_TEST(ep.getHeight() == 8);

    std::optional<Tile> first_empty = ep.getBoardState().at({0, 0});
    std::optional<Tile> p2_start = ep.getBoardState().at({5, 2});

    BOOST_TEST(!first_empty.has_value());
    BOOST_TEST(p2_start.has_value());
    BOOST_TEST(p2_start.value().is_special);
    BOOST_TEST(p2_start.value().owner == PLAYER_P2);

    Board lp = Board("Lakefront Property", parse_fin_state(lakefront_property));

    BOOST_TEST(lp.getWidth() == 16);
    BOOST_TEST(lp.getHeight() == 16);

    // Make sure it doesn't contain the empty square in the middle
    BOOST_TEST(!lp.getBoardState().contains({6, 6}));

    first_empty = lp.getBoardState().at({0, 0});
    p2_start = lp.getBoardState().at({12, 3});

    BOOST_TEST(!first_empty.has_value());
    BOOST_TEST(p2_start.has_value());
    BOOST_TEST(p2_start.value().is_special);
    BOOST_TEST(p2_start.value().owner == PLAYER_P2);
}

BOOST_AUTO_TEST_CASE(test_invalid_fin_strings) {
    BoardState b;

    // Can't have a number without a following value
    try {
        b = parse_fin_state("727/x");
        BOOST_TEST(false, "didn't fail line with unterminated number");
    } catch (std::exception& e) {}

    // Can't use invalid characters
    try {
        b = parse_fin_state("727wysi/x");
        BOOST_TEST(false, "didn't fail line with invalid characters");
    } catch (std::exception& e) {}

    // Numbers must not start with 0 character
    try {
        b = parse_fin_state("0727f/x");
        BOOST_TEST(false, "didn't fail line with invalid number");
    } catch (std::exception& e) {}

    try {
        b = parse_fin_state("0f/x");
        BOOST_TEST(false, "didn't fail line with invalid number");
    } catch (std::exception& e) {}


}
