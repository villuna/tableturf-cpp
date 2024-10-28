#include <iostream>
#include <print>
#include "client.hpp"
#include "protocol.hpp"
#include "raylib.h"
#include <boost/asio.hpp>
#include <deque>

namespace asio = boost::asio;

int main(int argc, char** argv) {
    asio::io_context ctx;
    std::deque<ServerMessage> messages;
    Client c(messages, ctx);

    InitWindow(800, 400, "hello raylib cpp");
    SetTargetFPS(60);

    std::string instructions = "C to connect. D to disconnect. S to send.";
    std::string status = "Disconnected";
    std::string recieved = "";
    
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_C)) {
            c.connect();
        } else if (IsKeyPressed(KEY_D)) {
            c.disconnect();
        } else if (IsKeyPressed(KEY_S)) {
            c.queue_send(HelloServer { .info = PublicPlayerInfo { .name = "villuna" }});
        }

        if (c.is_online()) {
            status = "Connected"; 
        } else {
            status = "Disconnected";
            recieved = "";
        }

        if (!messages.empty()) {
            ServerMessage msg = messages.front();
            messages.pop_front();
            recieved = "Recieved!: " + server_message_to_json(msg);
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawFPS(10, 10);

        DrawText(instructions.c_str(), 200, 200, 20, LIGHTGRAY);
        DrawText(status.c_str(), 200, 250, 20, LIGHTGRAY);
        DrawText(recieved.c_str(), 200, 300, 20, LIGHTGRAY);

        EndDrawing();

        // We run the async context at the end of every frame
        // this way we can have our event loops for both the game and the network, on the one thread
        ctx.poll();
        ctx.restart();
    }

    ctx.stop();

    return 0;
}
