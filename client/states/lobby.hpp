#pragma once
#include "../game.hpp"
#include "../ui/button.hpp"

class Lobby : public GameState {
    Button button_back;
    std::string server_msg;
    bool sent_hello { false };

public:
    Lobby(Game& parent);
    ~Lobby() override;

    StateTransition update() override;
    void draw() override;
};
