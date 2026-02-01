#include "GameScreen.h"

// This is the actual definition + initialization
static GameScreen CurrentScreen = GameScreen::SCREEN_MAINMENU;

GameScreen GetCurrentScreen() {
    return CurrentScreen;
}

void SetCurrentScreen(GameScreen screen) {
    CurrentScreen = screen;
}