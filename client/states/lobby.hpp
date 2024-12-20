#pragma once
#include "../game.hpp"
#include "../ui/ui.hpp"

class Lobby : public GameState {
    Button button_back;
    Button button_find_game;

public:
    Lobby(Game& parent);

    StateTransition update() override;
    void draw() override;
};
