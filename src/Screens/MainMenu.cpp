#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include "MainMenu.h"

// Camera + player
Camera3D camera = {0};
Vector3 PlayerPosition = {0, 0, 0};

// Flags + settings
bool initialized = true;

static float CameraYaw = 0.0f;
static float CameraPitch = 0.0f;
static float MoveSpeed = 5.0f;

void MainMenu()
{
    // First run only
    if (initialized) {
        // Camera initial setup
        camera.position = {0.0f, 3.0f, -6.0f};
        camera.target = {0.0f, 1.0f, 0.0f};
        camera.up = {0.0f, 1.0f, 0.0f};
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;

        PlayerPosition = {0, 0, 0};

        DisableCursor();
        initialized = false;
    }

    // Mouse look
    Vector2 MouseDelta = GetMouseDelta();
    float sensitivity = 0.2f;
    float DeltaTime = GetFrameTime();

    CameraYaw += MouseDelta.x * sensitivity * DEG2RAD;
    CameraPitch += MouseDelta.y * sensitivity * DEG2RAD;

    CameraPitch = Clamp(CameraPitch, -45.0f * DEG2RAD, 45.0f * DEG2RAD);

    // Movement directions (XZ only)
    Vector3 forward = {
        cosf(CameraYaw),
        0.0f,
        sinf(CameraYaw)};

    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, {0, 1, 0}));

    Vector3 move = {0, 0, 0};

    if (IsKeyDown(KEY_W))
        move = Vector3Add(move, forward);
    if (IsKeyDown(KEY_S))
        move = Vector3Subtract(move, forward);
    if (IsKeyDown(KEY_D))
        move = Vector3Add(move, right);
    if (IsKeyDown(KEY_A))
        move = Vector3Subtract(move, right);

    if (Vector3Length(move) > 0.001f)
    {
        move = Vector3Normalize(move);
        move = Vector3Scale(move, MoveSpeed * DeltaTime);
        PlayerPosition = Vector3Add(PlayerPosition, move);
    }

    // --- THIRD PERSON CAMERA RIG ---
    float CamDistance = 6.0f;
    float CamHeight = 2.0f;

    Vector3 CamOffset = {
        -cosf(CameraYaw) * CamDistance,
        CamHeight,
        -sinf(CameraYaw) * CamDistance};

    camera.position = Vector3Add(PlayerPosition, CamOffset);

    // Camera looks at player with pitch applied
    camera.target = Vector3Add(
        PlayerPosition,
        Vector3 {
            0.0f,
            1.0f - CameraPitch * 2.0f, 
            0.0f
        }
    );

    BeginMode3D(camera);

    ClearBackground(WHITE);

    // Draw player
    DrawCubeV(PlayerPosition, {1.0f, 2.0f, 1.0f}, RED);

    // Draw objects
    DrawCube({3.0f, 0.0f, 3.0f}, 1, 1, 1, ORANGE);
    DrawCube({-3.0f, 1.0f, -3.0f}, 1, 1, 1, BLACK);
    DrawCube({3.0f, 1.0f, -3.0f}, 1, 1, 1, YELLOW);
    DrawCube({-3.0f, 2.0f, 3.0f}, 2, 2, 2, BLUE);

    EndMode3D();
}