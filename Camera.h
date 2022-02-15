#pragma once
#include "Vector3.h"

class Camera
{
public:
    Camera();
    Camera(Vector3f position, Vector3f target, Vector3f up, float vfov, float aspect, float aperture, float focusDistance);

    void ProcessInput();
    void Update();

    const Vector3f& GetLocalFrontAxis() const;
    const Vector3f& GetLocalRightAxis() const;
    const Vector3f& GetLocalUpAxis() const;

    Vector3f lower_left_corner;
    Vector3f horizontal;
    Vector3f vertical;
    Vector3f position;
    Vector3f target;
    float vfov;

    float aspect;

    float moveSpeed;
    float boostSpeed;
    float rotateSpeed;
    float lensRadius;
    float focusDistance;

private:
    float yaw, pitch;
    Vector3f front;
    Vector3f right;
    Vector3f up;
};