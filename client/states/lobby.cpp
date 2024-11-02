#include "lobby.hpp"
#include "game.hpp"
#include "protocol.hpp"
#include "raylib.h"
#include "ui/ui.hpp"
#include <iostream>

Lobby::Lobby(Game& parent) :
   GameState { parent },
   button_back(20, parent.config.window_size.second - 50 - BUTTON_VPADDING, "<- Back", 30),
   name_box(100, 100, 10, 50)
{
    parent.client.connect();
}

Lobby::~Lobby() {
    std::cout << "Lobby: attempting to disconnect" << std::endl;
    parent.client.disconnect();
}

StateTransition Lobby::update() {
    name_box.update();

    if (button_back.is_clicked()) {
        return trans::Pop {};
    }

    if (IsKeyPressed(KEY_ENTER) && !name_box.get_contents().empty() && parent.client.is_connected()) {
        parent.client.queue_send(HelloServer {
            .info = PublicPlayerInfo { .name = name_box.get_contents() }
        });
    }

    if (!parent.messages.empty()) {
        ServerMessage& msg = parent.messages.front();
        server_msg = "Server said: " + server_message_to_json(msg);
        parent.messages.pop_front();
    }

    return trans::None {};
}

void Lobby::draw() {
    ClearBackground(RAYWHITE);

    const char* message;
    Color colour = BLACK;

    if (parent.client.is_connected()) {
        message = "Connected";
    } else if (parent.client.is_online()) {
        message = "Connecting...";
    } else {
        message = "Connection failed - please try again";
        colour = RED;
    }

    name_box.draw();

    DrawText("Enter your name and press enter", 100, 200, 40, DARKGRAY);
    DrawText(message, 100, 300, 40, colour);
    DrawText(server_msg.c_str(), 100, 400, 40, DARKGRAY);

    button_back.draw();
}
