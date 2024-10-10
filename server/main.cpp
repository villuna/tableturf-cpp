#include <print>
#include "card.hpp"

int main(int argc, char** argv) {
    auto x = read_card_map("data/resources/cards.yaml");

    for (auto i : x) {
        std::println("id: {}, card name: {}", i.first, i.second.name);
    }

    std::println("Hello, server!");
    return 0;
}
