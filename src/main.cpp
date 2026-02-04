
#include "raylib.h"

int main() {
    InitWindow(800, 450, "C++ IS FUN");

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello world", 330, 200, 20, BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}