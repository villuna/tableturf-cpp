#include <print>
#include <boost/asio.hpp>
#include <deque>

#include "client.hpp"
#include "protocol.hpp"
#include "raylib.h"
#include "config.hpp"

namespace asio = boost::asio;

class Game {
    asio::io_context ctx;
    std::deque<ServerMessage> messages;
    Client c;
    Config config;

public:
    Game(int argc, char* argv[]) :
        ctx{},
        messages{},
        c(messages, ctx),
        config(Config::read_config())
    {
        InitWindow(config.window_size.first, config.window_size.second, "Tableturf++");
        SetTargetFPS(config.max_fps);
    }

    void run() {
        while (!WindowShouldClose()) {
            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawFPS(10, 10);

            DrawText("Tableturf++", 200, 200, 20, LIGHTGRAY);

            EndDrawing();

            // We run the async context at the end of every frame
            // this way we can have our event loops for both the game and the network, on the one thread
            ctx.poll();
            ctx.restart();
        }   

        ctx.stop();
    }
};

int main(int argc, char* argv[]) {
    Game g(argc, argv);
    g.run();

    return 0;
}
