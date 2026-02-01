#include "raylib.h"

int main() {
    InitWindow(1920, 1200, "Cyphon");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}