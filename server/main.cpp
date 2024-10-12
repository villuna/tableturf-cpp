#include <iostream>
#include "card.hpp"

int main(int argc, char** argv) {
    auto x = read_card_map("data/resources/cards.yaml");

    for (auto i : x) {
        std::cout << "id: " << i.first << ", card name: " << i.second.name << std::endl;
    }

    std::cout << "Hello, server!" << std::endl;

    return 0;
}
