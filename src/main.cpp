#include "raylib.h"
#include "Global/GameScreen.h"
#include "Screens/MainMenu.h"

int main()
{
    InitWindow(1920, 1200, "Cyphon");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        GameScreen screen = GetCurrentScreen(); 

        BeginDrawing();
        ClearBackground(BLACK);

        switch (screen) {
        case GameScreen::SCREEN_MAINMENU:
            MainMenu();
            break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
