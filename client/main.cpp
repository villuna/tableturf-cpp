#include <print>
#include "raylib.h"

int main(int argc, char** argv) {
    InitWindow(800, 400, "hello raylib cpp");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawFPS(10, 10);
        DrawText("Hello raylib cpp", 200, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    return 0;
}
