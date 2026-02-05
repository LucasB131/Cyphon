#include "raylib.h"
#include "GameState.h"
#include "Colors/Colors.h"
#include "Types/GameScreen.h"
#include "Types/SaveData.h"
#include "SaveFunctions/SaveSystem.h"
#include "Screens/SKLScreen.h"
#include "Screens/StartMenu.h"
#include "Screens/NewAvatar.h"
#include "Screens/CreateAvatar.h"
#include "Screens/Sandbox.h"

int main() {
    InitWindow(1920, 1200, "Worlds of Enchant");
    SetTargetFPS(60);

    // Initialize global game state
    gGame.cinzel = LoadFontEx("fonts/CinzelDecorative-Bold.ttf", 128, 0, 0);
    gGame.currentScreen = SCREEN_SKL;
    gGame.slot = 0;
    gGame.save = SaveData();

    while (!WindowShouldClose()) {

        BeginDrawing();

        if (gGame.currentScreen == SCREEN_SKL) {
            SKLScreen();
        }
        else if (gGame.currentScreen == SCREEN_MENU) {
            StartMenu();
        }
        else if (gGame.currentScreen == SCREEN_NEWAVATAR) {
            NewAvatar();
        }
        else if (gGame.currentScreen == SCREEN_CREATEAVATAR) {
            CreateAvatar();
        }
        else if (gGame.currentScreen == SCREEN_SANDBOX) {
            Sandbox();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}