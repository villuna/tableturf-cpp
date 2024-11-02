#pragma once
#include "../game.hpp"
#include "../ui/ui.hpp"

class Lobby : public GameState {
    Button button_back;
    TextBox name_box;
    std::string server_msg;
    bool sent_hello { false };

public:
    Lobby(Game& parent);
    ~Lobby() override;

    StateTransition update() override;
    void draw() override;
};
