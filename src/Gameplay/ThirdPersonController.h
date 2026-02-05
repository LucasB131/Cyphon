#pragma once
#include "raylib.h"
#include "raymath.h"

struct ThirdPersonState
{
    Vector3 playerPos;
    float playerSpeed;

    float yaw;
    float pitch;

    Vector3 smoothMoveDir;
};

void InitThirdPersonState(ThirdPersonState &state, Vector3 startPos, float speed);

void UpdateThirdPersonCamera(ThirdPersonState &state, Camera3D &camera, float dt);

void UpdateThirdPersonMovement(ThirdPersonState &state, float dt);