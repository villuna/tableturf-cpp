#include "lobby.hpp"
#include "game.hpp"
#include "protocol.hpp"
#include "raylib.h"
#include "ui/button.hpp"
#include <iostream>

Lobby::Lobby(Game& parent) :
   GameState { parent },
   button_back(20, parent.config.window_size.second - 50 - BUTTON_VPADDING, "<- Back", 30)
{
    parent.client.connect();
}

Lobby::~Lobby() {
    std::cout << "Lobby: attempting to disconnect" << std::endl;
    parent.client.disconnect();
}

StateTransition Lobby::update() {
    if (button_back.is_clicked()) {
        return trans::Pop {};
    }

    if (parent.client.is_connected()) {
        if (!sent_hello) {
            parent.client.queue_send(HelloServer { .info = PublicPlayerInfo { .name = "villuna" }});
            sent_hello = true;
        }
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

    DrawText(message, 100, 100, 40, colour);
    DrawText(server_msg.c_str(), 100, 200, 40, DARKGRAY);

    button_back.draw();
}
