#pragma once
#include "raylib.h"
#include "raymath.h"

enum ProjectileType {
    PROJECTILE_FIREBASE,
    PROJECTILE_ICEBASE
};

struct Projectile {
    ProjectileType type;
    Vector3 position;
    Vector3 velocity;
    float lifetime;
    float damage;
    bool active;
};