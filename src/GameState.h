#pragma once
#include "raylib.h"
#include "Types/GameScreen.h"
#include "Types/SaveData.h"

struct GameState {
    Font cinzel;
    GameScreen currentScreen;
    SaveData save;
    int slot;
};

extern GameState gGame;