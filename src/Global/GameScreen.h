#pragma once

enum class GameScreen {
    SCREEN_MAINMENU
};

// Returns the current screen
GameScreen GetCurrentScreen();

// Sets the current screen
void SetCurrentScreen(GameScreen screen);