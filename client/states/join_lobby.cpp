#include "join_lobby.hpp"
#include "game.hpp"
#include "protocol.hpp"
#include "raylib.h"
#include "states/lobby.hpp"
#include "ui/ui.hpp"
#include <memory>

JoinLobby::JoinLobby(Game& parent) :
   GameState { parent },
   button_back(20, parent.config.window_size.second - 50 - BUTTON_VPADDING, "<- Back", 30),
   name_box(100 + MeasureText("Username: ", 50), 100, 10, 50)
{
    parent.client.connect();
}

StateTransition JoinLobby::update() {
    name_box.update();

    if (button_back.is_clicked()) {
        parent.client.disconnect();
        return trans::Pop {};
    }

    if (IsKeyPressed(KEY_ENTER) && !name_box.get_contents().empty() && parent.client.is_connected()) {
        parent.client.queue_send(HelloServer {
            .info = PublicPlayerInfo { .name = name_box.pop_contents() }
        });
    }

    if (!parent.messages.empty()) {
        ServerMessage& msg = parent.messages.front();
        parent.messages.pop_front();

        if (std::holds_alternative<HelloClient>(msg)) {
            return trans::Swap { std::make_unique<Lobby>(parent) };
        }
    }

    return trans::None {};
}

void JoinLobby::draw() {
    ClearBackground(RAYWHITE);

    if (parent.client.is_connected()) {
        DrawText("Username: ", 100, 100, 50, BLACK);
        name_box.draw();
        DrawText("Press enter to join the loby!", 100, 200, 40, BLACK);
    } else if (parent.client.is_online()) {
        DrawText("Connecting... Please wait...", 100, 200, 40, BLACK);
    } else {
        DrawText("Can't connect to the server :/\nGo back to the menu and try again", 100, 200, 40, RED);
    }

    button_back.draw();    
}
