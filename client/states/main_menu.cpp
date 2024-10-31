#include "main_menu.hpp"
#include "states/info.hpp"
#include "states/lobby.hpp"
#include <memory>

const int TITLE_FONT_SIZE = 70;
const char* TITLE = "Tableturf++";

MainMenu::MainMenu(Game& parent) :
    GameState { parent }
{
    InitWindow(parent.config.window_size.first, parent.config.window_size.second, "Tableturf++");
    SetTargetFPS(parent.config.max_fps);
    SetExitKey(KEY_NULL);

    int offset = MeasureText(TITLE, TITLE_FONT_SIZE) / 2;
    int title_x = 1280 / 2 - offset;

    button_single = Button(title_x, 250, "Singleplayer", 50);
    button_multi = Button(title_x, 330, "Multiplayer", 50);
    button_settings = Button(title_x, 410, "Settings", 50);
    button_info = Button(title_x, 490, "Info", 50);
    button_exit = Button(title_x, 570, "Exit", 50);
}

StateTransition MainMenu::update() {
    if (button_exit.is_clicked()) {
        return trans::Exit{};
    } else if (button_multi.is_clicked()) {
        return trans::Push { .state = std::make_unique<Lobby>(parent) };
    } else if (button_info.is_clicked()) {
        return trans::Push { .state = std::make_unique<Info>(parent) };
    }

    return trans::None{};
}

void MainMenu::draw() {
    int offset = MeasureText(TITLE, TITLE_FONT_SIZE) / 2;
    int title_x = 1280 / 2 - offset;

    ClearBackground(RAYWHITE);
    DrawFPS(10, 10);

    DrawText(TITLE, title_x, 100, TITLE_FONT_SIZE, DARKGRAY);

    button_single.draw();
    button_multi.draw();
    button_settings.draw();
    button_info.draw();
    button_exit.draw();
}
