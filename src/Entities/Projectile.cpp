#include "Projectile.h"
#include "raylib.h"
#include "raymath.h"

void UpdateProjectile(Projectile& p, float dt) {
    if (!p.active) return;

    // Move
    p.position.x += p.velocity.x * dt;
    p.position.y += p.velocity.y * dt;
    p.position.z += p.velocity.z * dt;

    // Lifetime countdown
    p.lifetime -= dt;
    if (p.lifetime <= 0) {
        p.active = false;
    }
}

void DrawProjectile(const Projectile& p) {
    if (!p.active) return;

    // Placeholder: draw a small sphere
    DrawSphere(p.position, 0.1f, YELLOW);
}