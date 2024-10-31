#include "game.hpp"
#include "states/main_menu.hpp"
#include <raylib.h>
#include <memory>
#include <variant>

Game::Game(int argc, char* argv[]) :
    messages{},
    ctx{},
    state{},
    config(Config::read_config()),
    client(messages, ctx)
{
    state.emplace_back(new MainMenu(*this));
    InitWindow(config.window_size.first, config.window_size.second, "Tableturf++");
    SetTargetFPS(config.max_fps);
    SetExitKey(KEY_NULL);
}

void Game::run() {
    while (!WindowShouldClose()) {
        BeginDrawing();
            state.back()->draw();
        EndDrawing();

        StateTransition trans = state.back()->update();

        if (std::holds_alternative<trans::Exit>(trans)) {
            break;
        } else if (std::holds_alternative<trans::Pop>(trans)) {
            if (state.size() == 1) {
                break;
            } else {
                state.pop_back();
            }
        } else if (std::holds_alternative<trans::Push>(trans)) {
            trans::Push& p = std::get<trans::Push>(trans);
            state.push_back(std::move(p.state));
        } else if (std::holds_alternative<trans::Swap>(trans)) {
            trans::Swap& s = std::get<trans::Swap>(trans);
            state.pop_back();
            state.push_back(std::move(s.state));
        }

        // We run the async context at the end of every frame
        // this way we can have our event loops for both the game and the network, on the one thread
        ctx.poll();
        ctx.restart();
    }   

    ctx.stop();
    CloseWindow();
}
