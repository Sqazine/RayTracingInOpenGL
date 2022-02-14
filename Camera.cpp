#include "Camera.h"
#include "Input.h"
#include "Timer.h"
#include <glm/glm.hpp>
Camera::Camera() {}
Camera::Camera(Vector3f position, Vector3f target, Vector3f up, float vfov, float aspect, float aperture, float focus_dist)
    : position(position), target(target), vfov(vfov), aspect(aspect), moveSpeed(10.0f), rotateSpeed(10.0f), boostSpeed(100.0f), yaw(-90.0f), pitch(0.0f), focus_dist(focus_dist)
{
    float theta = Math::ToRadian(vfov);
    float h = std::atan(theta / 2.0f);
    float viewport_height = 2.0f * h;
    float viewport_width = aspect * viewport_height;

    front = Vector3f::Normalize(target - position);
    right = Vector3f::Normalize(Vector3f::Cross(front, up));
    selfUp = Vector3f::Normalize(Vector3f::Cross(right, front));

    horizontal = focus_dist * viewport_width * right;
    vertical = focus_dist * viewport_height * selfUp;
    lower_left_corner = position - horizontal / 2.0f - vertical / 2.0f + front * focus_dist;

    lens_radius = aperture / 2.0f;
}

void Camera::ProcessInput()
{
    float speed = 0;
    if (Input::GetKeyboard()->GetKeyState(SDL_SCANCODE_LSHIFT) == ButtonState::HOLD)
        speed = (moveSpeed + boostSpeed) * Timer::GetDeltaTime();
    else
        speed = moveSpeed * Timer::GetDeltaTime();

    if (Input::GetKeyboard()->GetKeyState(SDL_SCANCODE_W) == ButtonState::HOLD)
        position += front * speed;
    if (Input::GetKeyboard()->GetKeyState(SDL_SCANCODE_S) == ButtonState::HOLD)
        position -= front * speed;
    if (Input::GetKeyboard()->GetKeyState(SDL_SCANCODE_A) == ButtonState::HOLD)
        position -= right * speed;
    if (Input::GetKeyboard()->GetKeyState(SDL_SCANCODE_D) == ButtonState::HOLD)
        position += right * speed;
    if (Input::GetKeyboard()->GetKeyState(SDL_SCANCODE_E) == ButtonState::HOLD)
        position += selfUp * speed;
    if (Input::GetKeyboard()->GetKeyState(SDL_SCANCODE_Q) == ButtonState::HOLD)
        position -= selfUp * speed;

    yaw += Input::GetMouse()->GetMousePos().x * rotateSpeed * Timer::GetDeltaTime();
    pitch -= Input::GetMouse()->GetMousePos().y * rotateSpeed * Timer::GetDeltaTime();

    //Avoid camera to have visual inversion during pitch, so  limit the pitch Angle
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    front = Vector3f::Normalize(front + Vector3f(cos(Math::ToRadian(pitch)) * cos(Math::ToRadian(yaw)), sin(Math::ToRadian(pitch)), cos(Math::ToRadian(pitch)) * sin(Math::ToRadian(yaw))));
    right = Vector3f::Normalize(Vector3f::Cross(front, Vector3f(0.0f, 1.0f, 0.0f)));
    selfUp = Vector3f::Normalize(Vector3f::Cross(right, front));

    target = position + front;

    float mouseScrollLength = Input::GetMouse()->GetMouseScrollWheel().y;
    vfov -= mouseScrollLength * Timer::GetDeltaTime() * boostSpeed;
}

void Camera::Update()
{
    float theta = Math::ToRadian(vfov);
    float h = std::atan(theta / 2.0f);
    float viewport_height = 2.0f * h;
    float viewport_width = aspect * viewport_height;

    horizontal = focus_dist * viewport_width * right;
    vertical = focus_dist * viewport_height * selfUp;
    lower_left_corner = position - horizontal / 2.0f - vertical / 2.0f + front * focus_dist;
}