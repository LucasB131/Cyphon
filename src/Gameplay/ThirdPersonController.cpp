#include "ThirdPersonController.h"

void InitThirdPersonState(ThirdPersonState &state, Vector3 startPos, float speed)
{
    state.playerPos = startPos;
    state.playerSpeed = speed;
    state.yaw = 0.0f;
    state.pitch = 0.0f;
    state.smoothMoveDir = {0.0f, 0.0f, 0.0f};
}

void UpdateThirdPersonCamera(ThirdPersonState &state, Camera3D &camera, float dt)
{
    float mouseX = GetMouseDelta().x;
    float mouseY = GetMouseDelta().y;

    state.yaw += mouseX * 0.003f;
    state.pitch -= mouseY * 0.003f;

    state.pitch = Clamp(state.pitch, -1.2f, 1.2f);

    Vector3 lookDir = {
        cosf(state.pitch) * cosf(state.yaw),
        sinf(state.pitch),
        cosf(state.pitch) * sinf(state.yaw)};
    lookDir = Vector3Normalize(lookDir);

    Vector3 right = {
        -sinf(state.yaw),
        0.0f,
        cosf(state.yaw)};
    right = Vector3Normalize(right);

    Vector3 forwardYaw = {
        cosf(state.yaw),
        0.0f,
        sinf(state.yaw)};
    forwardYaw = Vector3Normalize(forwardYaw);

    const float camDist = 4.5f;
    const float shoulderOffset = 1.0f;
    const float camYOffset = 1.5f;

    Vector3 camPos;
    camPos.x = state.playerPos.x - lookDir.x * camDist + right.x * shoulderOffset;
    camPos.y = state.playerPos.y + camYOffset;
    camPos.z = state.playerPos.z - lookDir.z * camDist + right.z * shoulderOffset;

    Vector3 camTarget;
    camTarget.x = state.playerPos.x + lookDir.x * 100.0f;
    camTarget.y = state.playerPos.y + lookDir.y * 100.0f;
    camTarget.z = state.playerPos.z + lookDir.z * 100.0f;

    camera.position = camPos;
    camera.target = camTarget;
    camera.up = {0.0f, 1.0f, 0.0f};
}

void UpdateThirdPersonMovement(ThirdPersonState &state, float dt)
{
    Vector3 forwardYaw = {
        cosf(state.yaw),
        0.0f,
        sinf(state.yaw)};
    forwardYaw = Vector3Normalize(forwardYaw);

    Vector3 right = {
        -sinf(state.yaw),
        0.0f,
        cosf(state.yaw)};
    right = Vector3Normalize(right);

    Vector3 desiredMove = {0.0f, 0.0f, 0.0f};

    if (IsKeyDown(KEY_W))
        desiredMove = Vector3Add(desiredMove, forwardYaw);
    if (IsKeyDown(KEY_S))
        desiredMove = Vector3Subtract(desiredMove, forwardYaw);
    if (IsKeyDown(KEY_A))
        desiredMove = Vector3Subtract(desiredMove, right);
    if (IsKeyDown(KEY_D))
        desiredMove = Vector3Add(desiredMove, right);

    if (Vector3Length(desiredMove) > 0.01f)
        desiredMove = Vector3Normalize(desiredMove);

    state.smoothMoveDir = Vector3Lerp(state.smoothMoveDir, desiredMove, 12.0f * dt);

    state.playerPos = Vector3Add(state.playerPos,
                                 Vector3Scale(state.smoothMoveDir, state.playerSpeed * dt));
}