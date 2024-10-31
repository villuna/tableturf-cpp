#pragma once

#include "../ui/button.hpp"
#include "../game.hpp"

class Info : public GameState {
    Button button_back;

public:
    Info(Game& parent);

    StateTransition update() override;
    void draw() override;
};
