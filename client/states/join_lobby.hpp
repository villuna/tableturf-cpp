#pragma once
#include "../game.hpp"
#include "../ui/ui.hpp"

class JoinLobby : public GameState {
    Button button_back;
    TextBox name_box;
    bool sent_hello { false };

public:
    JoinLobby(Game& parent);

    StateTransition update() override;
    void draw() override;
};
