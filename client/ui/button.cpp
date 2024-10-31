#include "button.hpp"
#include "raylib.h"

void Button::draw() {
    if (!is_hovered()) {
        DrawRectangle(x, y, width, height, VIOLET);
    } else {
        DrawRectangle(x, y, width, height, PURPLE);
    }

    DrawText(label.c_str(), x + BUTTON_HPADDING, y + BUTTON_VPADDING, font_size, WHITE);
}

bool Button::is_hovered() {
    Vector2 pos = GetMousePosition();

    return pos.x >= x && pos.x <= x + width && pos.y >= y && pos.y <= y + height;
}

bool Button::is_clicked() {
    return is_hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
