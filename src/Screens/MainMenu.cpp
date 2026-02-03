#include "raylib.h"
#include "MainMenu.h"

// Global camera
Camera3D camera = {0};

bool initialized = true;

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

    // Update camera view
    UpdateCameraPro (
        &camera,
        Vector3{0, 0, 0},
        Vector3{MouseDelta.x * sensitivity, MouseDelta.y * sensitivity, 0},
        0.0f
    );

    BeginMode3D(camera);

    ClearBackground(WHITE);
    DrawCube({3.0f, 0.0f, 3.0f}, 1.0f, 1.0f, 1.0f, RED);

    EndMode3D();
}