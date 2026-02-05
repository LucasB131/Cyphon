#pragma once
#include "raylib.h"
#include "raymath.h"
#include <string>

enum EnemyType {
    ENEMY_ICEMINION,
    ENEMY_FIREMINION
};

struct Enemy {
    EnemyType type;
    Vector3 position;
    float health;
    float speed;
    float damage;
    bool active;
};