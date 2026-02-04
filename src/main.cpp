
#include "raylib.h"
#include "iostream"

int main() {
    InitWindow(1080, 800, "MINESWEEPER");

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello world", 130, 200, 100, BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}