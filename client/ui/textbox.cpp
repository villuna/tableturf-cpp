#include "textbox.hpp"
#include "raylib.h"
#include <string>

const int TEXTBOX_HPADDING = 5;
const int TEXTBOX_VPADDING = 3;

const Color TEXTBOX_DEFAULT_COLOUR = LIGHTGRAY;
const Color TEXTBOX_DEFAULT_TEXT_COLOUR = BLUE;

TextBox::TextBox(int x, int y, int char_limit, int font_size) :
    char_limit(char_limit),
    font_size(font_size)
{
    // Might seem weird that I'm doing this thing where I fill the contents string with Gs
    // I'll need to make a max-length string to measure how long the textbox will be, and this
    // way we only ever need one allocation for the `contents` buffer.

    // Make space for extra underscore and null terminator
    contents.reserve(char_limit + 2);
    for (int i = 0; i < char_limit; i++)
        contents.push_back('G');

    int textbox_width = MeasureText(contents.c_str(), font_size);
    contents.clear();

    bounds.x = x;
    bounds.y = y;
    bounds.width = textbox_width + TEXTBOX_HPADDING * 2;
    bounds.height = font_size + TEXTBOX_VPADDING * 2;
}

void TextBox::update() {
    // Let the user write to the textbox (only if it isn't full)
    
    // GetCharPressed is stateful and will return a code > 0 while
    // there are more keypresses to process
    for (int key = GetCharPressed(); (key > 0 && contents.length() < char_limit); key = GetCharPressed()) {
        // only allow printable ascii
        // i know i probably should support japanese input but bro i'm not even sure if raylib supports japanese input
        if (key >= 32 && key <= 126) {
            auto c = static_cast<char>(key);
            contents.push_back(c);
        }
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !contents.empty()) {
        contents.pop_back();
    }
}

void TextBox::draw() {
    DrawRectangleRec(bounds, TEXTBOX_DEFAULT_COLOUR);

    bool draw_underscore = contents.length() < char_limit;

    if (draw_underscore)
        contents.push_back('_');

    DrawText(
        contents.c_str(), 
        bounds.x + TEXTBOX_HPADDING,
        bounds.y + TEXTBOX_VPADDING,
        font_size,
        TEXTBOX_DEFAULT_TEXT_COLOUR
    );

    if (draw_underscore)
        contents.pop_back();


    std::string limit = std::to_string(contents.length()) + "/" + std::to_string(char_limit);
    DrawText(limit.c_str(), bounds.x, bounds.y + bounds.height + TEXTBOX_VPADDING, 20, DARKGRAY);
}
