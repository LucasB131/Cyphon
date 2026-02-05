#include "SKLScreen.h"
#include "../GameState.h"

void SKLScreen() {

    static bool initialized = false;
    static double startTime = 0.0;

    // One‑time init
    if (!initialized) {
        startTime = GetTime();
        initialized = true;
    }

    double elapsed = GetTime() - startTime;

    // Skip intro if space is pressed
    if (IsKeyPressed(KEY_SPACE)) {
        gGame.currentScreen = SCREEN_MENU;
        initialized = false;
        return;
    }

    ClearBackground(BLACK);

    // Fade in
    if (elapsed >= 1 && elapsed < 3) {
        float opacity = (elapsed - 1.0f) * 125.0f;
        if (opacity > 255) opacity = 255;
        if (opacity < 0) opacity = 0;

        Color FadeInGold = {255, 212, 122, (unsigned char)opacity};
        DrawTextEx(gGame.cinzel, "SKL  PRODUCTIONS  PRESENTS",
                   {310, 500}, 100, 2, FadeInGold);
    }

    // Constant
    if (elapsed >= 3 && elapsed < 5) {
        Color ConstantGold = {255, 212, 122, 255};
        DrawTextEx(gGame.cinzel, "SKL  PRODUCTIONS  PRESENTS",
                   {310, 500}, 100, 2, ConstantGold);
    }

    // Fade out
    if (elapsed >= 5 && elapsed < 7) {
        float opacity = 255 - ((elapsed - 5.0f) * 125.0f);
        if (opacity > 255) opacity = 255;
        if (opacity < 0) opacity = 0;

        Color FadeOutGold = {255, 212, 122, (unsigned char)opacity};
        DrawTextEx(gGame.cinzel, "SKL  PRODUCTIONS  PRESENTS",
                   {310, 500}, 100, 2, FadeOutGold);
    }

    // Auto‑skip after 8 seconds
    if (elapsed > 8) {
        gGame.currentScreen = SCREEN_MENU;
        initialized = false;
    }
}