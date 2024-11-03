#pragma once


#include <raylib.h>
#include <string>
#include <utility>

class TextBox {
    std::string contents;
    Rectangle bounds;
    int char_limit;
    int font_size;

public:
    TextBox() {}
    TextBox(int x, int y, int char_limit, int font_size);

    void draw();
    void update();
    const std::string& get_contents() const { return contents; };
    std::string pop_contents() { return std::move(contents); };
};
