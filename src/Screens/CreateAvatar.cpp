#include "raylib.h"
#include "CreateAvatar.h"
#include "../GameState.h"
#include "../Types/GameScreen.h"
#include "../Types/SaveData.h"
#include "../Colors/Colors.h"
#include "../SaveFunctions/SaveSystem.h"
#include "../UI/Helpers.h"

// Global naming state (shared across calls)
bool nameavatar = true;

void CreateAvatar() {

    // Per-screen static state
    static bool initialized = false;
    static std::string nameBuffer = "";

    if (!initialized) {
        nameavatar = true;              
        LoadFromSlot(gGame.save, gGame.slot);   // load the correct save
        nameBuffer = gGame.save.name;           // prefill name
        initialized = true;
    }

    ClearBackground(CustomGold);

    // -------------------------------------------------
    // PHASE 1: NAME ENTRY
    // -------------------------------------------------
    if (nameavatar) {

        Rectangle nameavatarbox = {500, 400, 920, 400};
        DrawRectangleRounded(nameavatarbox, 0.25f, 16, CustomLavender);

        DrawTextCenteredAtPoint(gGame.cinzel, "What is your name?", 960, 500, 40, 2, BLACK);

        // --- TEXT INPUT ---
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125) {   // printable ASCII
                if (nameBuffer.length() < 16) {
                    nameBuffer.push_back((char)key);
                }
            }
            key = GetCharPressed();
        }

        // Backspace
        if (IsKeyPressed(KEY_BACKSPACE) && !nameBuffer.empty()) {
            nameBuffer.pop_back();
        }

        // Draw name + cursor
        std::string display = nameBuffer;
        bool cursorVisible = ((int)(GetTime() * 2) % 2) == 0;
        if (cursorVisible) display += "|";

        DrawTextCenteredAtPoint(gGame.cinzel, display.c_str(), 960, 700, 50, 2, RED);

        // Confirm name (block empty)
        if (IsKeyPressed(KEY_ENTER) && !nameBuffer.empty()) {
            gGame.save.name = nameBuffer;
            SaveToSlot(gGame.save, gGame.slot);   // persist name to save file
            nameavatar = false;                   // move to next phase
        }
    }

    // -------------------------------------------------
    // PHASE 2: AVATAR PREVIEW / CUSTOMIZATION
    // -------------------------------------------------
    else {

        Rectangle avatarbox = {100, 200, 600, 900};
        DrawRectangleRounded(avatarbox, 0.25f, 16, CustomLavender);
        DrawRectangleRoundedLinesEx(avatarbox, 0.25f, 16, 3.0f, BLACK);

        // Show chosen name
        DrawTextCenteredAtPoint(gGame.cinzel, gGame.save.name.c_str(), 400, 150, 60, 2, BLACK);

        // Finish and go to Sandbox
        if (IsKeyPressed(KEY_ENTER)) {
            initialized = false;          
            gGame.currentScreen = SCREEN_SANDBOX;
            return;
        }
    }
}