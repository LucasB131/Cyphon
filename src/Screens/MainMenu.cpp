#include "raylib.h"
#include "raymath.h"
#include <cmath>     
#include "MainMenu.h"

// Global camera
Camera3D camera = {0};

// Definitions
bool initialized = true;

static float CameraYaw = 0.0f;
static float CameraPitch = 0.0f;
static float MoveSpeed = 5.0f;

void MainMenu() {
    // First run through only
    if (initialized) {
        camera = {0}; 

        camera.position = {0.0f, 2.0f, 0.0f};
        camera.target = {1.0f, 2.0f, 1.0f};
        camera.up = {0.0f, 1.0f, 0.0f};
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;

        DisableCursor();

        initialized = false; 
    }

    // Cursor data for rotation
    Vector2 MouseDelta = GetMouseDelta();
    float sensitivity  = 0.2f;


    // Player movement detection, calculation and implementation
    // Ignore frame rate for movement
    float DeltaTime = GetFrameTime();

    // Detect change in rotation
    CameraYaw += MouseDelta.x * sensitivity * DEG2RAD;
    CameraPitch += MouseDelta.y * sensitivity * DEG2RAD;

    // Clamp pitch so you can't flip upside down
    CameraPitch = Clamp(CameraPitch, -89.0f * DEG2RAD, 89.0f * DEG2RAD);

    // Camera target along XZ plane
    Vector3 forward = {
        cosf(CameraYaw),
        0.0f,
        sinf(CameraYaw)
    };

    // Strafe direction
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, {0, 1, 0}));

    // Initialize movement vector
    Vector3 move = {0, 0, 0};

    // Compute movement vector value
    if (IsKeyDown(KEY_W))
        move = Vector3Add(move, forward);
    if (IsKeyDown(KEY_S))
        move = Vector3Subtract(move, forward);
    if (IsKeyDown(KEY_D))
        move = Vector3Add(move, right);
    if (IsKeyDown(KEY_A))
        move = Vector3Subtract(move, right);

    // Normalize vector and add speed
    if (Vector3Length(move) > 0.001f) {
        move = Vector3Normalize(move);
        move = Vector3Scale(move, MoveSpeed * DeltaTime);
        camera.position = Vector3Add(camera.position, move);
        camera.target = Vector3Add(camera.target, move);
    }

    // Recompute camera.target from yaw/pitch
    camera.target = Vector3Add(
        camera.position,
        Vector3 {
            cosf(CameraYaw) * cosf(CameraPitch),
            -sinf(CameraPitch),
            sinf(CameraYaw) * cosf(CameraPitch)
        }
    );

    BeginMode3D(camera);

    // Draw everything 
    ClearBackground(WHITE);
    DrawCube({3.0f, 0.0f, 3.0f}, 1.0f, 1.0f, 1.0f, RED);

    EndMode3D();
}