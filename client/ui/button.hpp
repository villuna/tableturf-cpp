#pragma once

#include "raylib.h"
#include <string>

#define BUTTON_VPADDING 7
#define BUTTON_HPADDING 15

class Button {
public:
    int x;
    int y;

    int font_size;

    int width;
    int height;

    std::string label;

    void draw();
    bool is_clicked();
    bool is_hovered();

    Button() :
        x(0),
        y(0),
        font_size(0),
        width(0),
        height(0),
        label()
    {}

    Button(int x, int y, std::string label, int font_size) :
        x(x),
        y(y),
        font_size(font_size),
        width(MeasureText(label.c_str(), font_size) + 2 * BUTTON_HPADDING),
        height(font_size + 2 * BUTTON_VPADDING),
        label(label)
    {}
};
