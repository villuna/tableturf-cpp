#pragma once

#include "../ui/button.hpp"
#include "../game.hpp"

class MainMenu : public GameState {
    Button button_single;
    Button button_multi;
    Button button_settings;
    Button button_info;
    Button button_exit;

public:
    MainMenu(Game& parent);

    StateTransition update() override;
    void draw() override;
};
