#include "Enemy.h"
#include "raylib.h"
#include "raymath.h"

void UpdateEnemy(Enemy& e, float dt) {
    if (!e.active) return;

    // Basic movement example
    e.position.x += e.speed * dt;

    // Death check
    if (e.health <= 0) {
        e.active = false;
    }
}

void DrawEnemy(const Enemy& e) {
    if (!e.active) return;

    // Placeholder: draw a sphere
    DrawSphere(e.position, 0.5f, RED);
}