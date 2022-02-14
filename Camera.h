#pragma once
#include "Vector3.h"

struct Camera
{
    Camera();
    Camera(Vector3f position, Vector3f target, Vector3f up, float vfov, float aspect, float aperture, float focus_dist);

    void ProcessInput();
    void Update();

    Vector3f lower_left_corner;
    Vector3f horizontal;
    Vector3f vertical;
    Vector3f position;
    Vector3f target;
    float vfov;

    float aspect;
    Vector3f front;
    Vector3f right;
    Vector3f selfUp;

    float moveSpeed;
    float boostSpeed;
    float rotateSpeed;
    float yaw, pitch;
    float lens_radius;
    float focus_dist;
};