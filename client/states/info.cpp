#include "info.hpp"
#include "game.hpp"
#include "raylib.h"

Info::Info(Game& parent) :
    GameState { parent },
    button_back(20, 20, "<- Back", 30)
{}

const char* info = R"(Written by villuna
Made with C++/Raylib/Boost.ASIO
Woomy!)";

void Info::draw() {
    ClearBackground(RAYWHITE);
    button_back.draw();
    DrawText(info, 100, 100, 50, BLACK);
}

StateTransition Info::update() {
    if (button_back.is_clicked()) {
        return trans::Pop{};
    }
    return trans::None{};
}
