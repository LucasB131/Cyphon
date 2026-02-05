#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include "../GameState.h"
#include "../Types/GameScreen.h"
#include "../Gameplay/ThirdPersonController.h"

// Player model + animation
static Model playermodel;
static ModelAnimation *anims = nullptr;
static int animCount = 0;
static int animFrame = 0;

// Projectile + enemy structs
struct Projectile
{
    Vector3 position;
    Vector3 velocity;
    float lifetime;
    bool active;
};

struct Enemy
{
    Vector3 position;
    float health;
    float shootTimer;
    bool alive;
};

void NewAvatar()
{
    // --- STATIC STATE ---
    static bool initialized = false;
    static Camera3D camera = {0};
    static ThirdPersonState tpState;

    static Projectile playerProj[100] = {0};
    static Projectile enemyProj[50] = {0};

    static Enemy iceEnemy;

    // Store base translations from frame 0 for all bones
    static bool baseTranslationsInit = false;
    static Vector3 baseTranslations[128]; // enough for your 65 bones

    float dt = GetFrameTime();

    // ============================================================
    // INITIALIZATION
    // ============================================================
    if (!initialized)
    {
        // Load model
        playermodel = LoadModel("assets/Characters/Base/ybot-idle-test2.glb");

        // Force simple shader + white diffuse
        Shader defaultShader = LoadShader(0, 0);
        for (int i = 0; i < playermodel.materialCount; i++)
        {
            playermodel.materials[i].shader = defaultShader;
            playermodel.materials[i].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
        }

        // Load animations
        anims = LoadModelAnimations("assets/Characters/Base/ybot-idle-test2.glb", &animCount);
        animFrame = 0;

        // Third-person state
        InitThirdPersonState(tpState, Vector3{0.0f, 0.0f, 5.0f}, 6.0f);

        // Camera
        camera.position = {0.0f, 2.0f, 6.0f};
        camera.target = {0.0f, 1.0f, 0.0f};
        camera.up = {0.0f, 1.0f, 0.0f};
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;

        // Projectiles
        for (int i = 0; i < 100; i++)
            playerProj[i].active = false;
        for (int i = 0; i < 50; i++)
            enemyProj[i].active = false;

        // Enemy
        iceEnemy.position = {0.0f, 1.0f, 0.0f};
        iceEnemy.health = 3.0f;
        iceEnemy.shootTimer = 0.0f;
        iceEnemy.alive = true;

        DisableCursor();
        initialized = true;
    }

    // ============================================================
    // EXIT BACK TO MENU
    // ============================================================
    if (IsKeyPressed(KEY_ENTER))
    {
        EnableCursor();
        initialized = false;
        gGame.currentScreen = SCREEN_CREATEAVATAR;
        return;
    }

    // ============================================================
    // CAMERA + MOVEMENT
    // ============================================================
    UpdateThirdPersonCamera(tpState, camera, dt);
    UpdateThirdPersonMovement(tpState, dt);

    float yaw = tpState.yaw;
    float pitch = tpState.pitch;

    Vector3 lookDir = {
        cosf(pitch) * cosf(yaw),
        sinf(pitch),
        cosf(pitch) * sinf(yaw)};
    lookDir = Vector3Normalize(lookDir);

    Vector3 right = {-sinf(yaw), 0.0f, cosf(yaw)};
    right = Vector3Normalize(right);

    Vector3 forwardYaw = {cosf(yaw), 0.0f, sinf(yaw)};
    forwardYaw = Vector3Normalize(forwardYaw);

    Vector3 up = {0.0f, 1.0f, 0.0f};

    // ============================================================
    // PROJECTILE SPAWN POSITION
    // ============================================================
    float scale = 1.0f;
    Vector3 handLocal = {0.35f * scale, 1.4f * scale, 0.3f * scale};

    Vector3 handWorldOffset = {
        right.x * handLocal.x + up.x * handLocal.y + forwardYaw.x * handLocal.z,
        right.y * handLocal.x + up.y * handLocal.y + forwardYaw.y * handLocal.z,
        right.z * handLocal.x + up.z * handLocal.y + forwardYaw.z * handLocal.z};

    Vector3 handWorldPos = Vector3Add(tpState.playerPos, handWorldOffset);

    // ============================================================
    // SHOOTING
    // ============================================================
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        for (int i = 0; i < 100; i++)
        {
            if (!playerProj[i].active)
            {
                playerProj[i].active = true;
                playerProj[i].position = handWorldPos;
                playerProj[i].velocity = Vector3Scale(lookDir, 20.0f);
                playerProj[i].lifetime = 2.0f;
                break;
            }
        }
    }

    // ============================================================
    // UPDATE PLAYER PROJECTILES
    // ============================================================
    for (int i = 0; i < 100; i++)
    {
        if (playerProj[i].active)
        {
            playerProj[i].position = Vector3Add(
                playerProj[i].position,
                Vector3Scale(playerProj[i].velocity, dt));
            playerProj[i].lifetime -= dt;
            if (playerProj[i].lifetime <= 0.0f)
                playerProj[i].active = false;
        }
    }

    // ============================================================
    // ENEMY SHOOTING
    // ============================================================
    if (iceEnemy.alive)
    {
        iceEnemy.shootTimer -= dt;

        if (iceEnemy.shootTimer <= 0.0f)
        {
            iceEnemy.shootTimer = 2.0f;

            for (int i = 0; i < 50; i++)
            {
                if (!enemyProj[i].active)
                {
                    enemyProj[i].active = true;
                    enemyProj[i].position = iceEnemy.position;

                    Vector3 dir = Vector3Normalize(Vector3Subtract(tpState.playerPos, iceEnemy.position));
                    enemyProj[i].velocity = Vector3Scale(dir, 10.0f);
                    enemyProj[i].lifetime = 3.0f;
                    break;
                }
            }
        }
    }

    // ============================================================
    // UPDATE ENEMY PROJECTILES
    // ============================================================
    for (int i = 0; i < 50; i++)
    {
        if (enemyProj[i].active)
        {
            enemyProj[i].position = Vector3Add(
                enemyProj[i].position,
                Vector3Scale(enemyProj[i].velocity, dt));
            enemyProj[i].lifetime -= dt;
            if (enemyProj[i].lifetime <= 0.0f)
                enemyProj[i].active = false;
        }
    }

    // ============================================================
    // PLAYER PROJECTILE → ENEMY COLLISION
    // ============================================================
    if (iceEnemy.alive)
    {
        BoundingBox enemyBox = {
            {iceEnemy.position.x - 0.5f, iceEnemy.position.y - 0.5f, iceEnemy.position.z - 0.5f},
            {iceEnemy.position.x + 0.5f, iceEnemy.position.y + 0.5f, iceEnemy.position.z + 0.5f}};

        for (int i = 0; i < 100; i++)
        {
            if (playerProj[i].active &&
                CheckCollisionBoxSphere(enemyBox, playerProj[i].position, 0.1f))
            {
                iceEnemy.health -= 1.0f;
                playerProj[i].active = false;

                if (iceEnemy.health <= 0.0f)
                    iceEnemy.alive = false;
            }
        }
    }

    // ============================================================
    // ANIMATION UPDATE WITH FRAME-0 TRANSLATION LOCK
    // ============================================================
    if (animCount > 0 && anims != nullptr)
    {
        ModelAnimation &idleAnim = anims[0];

        // Initialize base translations from frame 0 once
        if (!baseTranslationsInit && idleAnim.framePoses != nullptr)
        {
            for (int b = 0; b < idleAnim.boneCount; b++)
            {
                baseTranslations[b] = idleAnim.framePoses[0][b].translation;
            }
            baseTranslationsInit = true;
        }

        // For this frame, force all bone translations to frame-0 values
        if (baseTranslationsInit &&
            idleAnim.framePoses != nullptr &&
            animFrame >= 0 && animFrame < idleAnim.frameCount)
        {
            for (int b = 0; b < idleAnim.boneCount; b++)
            {
                idleAnim.framePoses[animFrame][b].translation = baseTranslations[b];
            }
        }

        // Apply animation normally (rotations still animate)
        UpdateModelAnimation(playermodel, idleAnim, animFrame);

        animFrame++;
        if (animFrame >= idleAnim.frameCount)
            animFrame = 0;
    }

    // ============================================================
    // DRAW
    // ============================================================
    ClearBackground(SKYBLUE);
    BeginMode3D(camera);

    DrawPlane(Vector3{0.0f, 0.0f, 0.0f}, Vector2{50.0f, 50.0f}, BROWN);

    // Character is 1.8 tall → center is 0.9
    // YBot origin is ~1.0 above feet → feet at playerPos.y - 0.1
    const float modelCenter = 0.9f;
    const float modelFeetOffset = 1.0f;

    Vector3 drawPos = {
        tpState.playerPos.x,
        tpState.playerPos.y + modelCenter + 6.0f, // ≈ playerPos.y - 0.1
        tpState.playerPos.z};

    float playerYawDegrees = -yaw * RAD2DEG + 90.0f;

    DrawModelEx(
        playermodel,
        drawPos,
        Vector3{0.0f, 1.0f, 0.0f},
        playerYawDegrees,
        Vector3{1.0f, 1.0f, 1.0f},
        WHITE);

    if (iceEnemy.alive)
        DrawCube(iceEnemy.position, 1.0f, 1.0f, 1.0f, SKYBLUE);

    for (int i = 0; i < 100; i++)
        if (playerProj[i].active)
            DrawSphere(playerProj[i].position, 0.1f, RED);

    for (int i = 0; i < 50; i++)
        if (enemyProj[i].active)
            DrawSphere(enemyProj[i].position, 0.1f, DARKBLUE);

    EndMode3D();

    int cx = GetScreenWidth() / 2;
    int cy = GetScreenHeight() / 2;
    DrawLine(cx - 10, cy, cx + 10, cy, WHITE);
    DrawLine(cx, cy - 10, cx, cy + 10, WHITE);
}