#include "lobby.hpp"
#include "protocol.hpp"
#include "raylib.h"

Lobby::Lobby(Game& parent) :
    GameState { parent },
    button_back(20, parent.config.window_size.second - 50 - BUTTON_VPADDING, "<- Back", 30),
    button_find_game(100, 100, "Join Match", 50)
{}

StateTransition Lobby::update() {
    if (button_back.is_clicked()) {
        parent.client.disconnect();
        return trans::Pop {};
    }

    if (button_find_game.is_clicked()) {
        parent.client.queue_send(FindGame {});
    }

    return trans::None {};
}

void Lobby::draw() {
    ClearBackground(RAYWHITE);

    button_back.draw();
    button_find_game.draw();
}
