#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
#include "App.h"
#include "GL/Shader.h"
#include "Shape.h"
#include <glm/glm.hpp>
#include "Camera.h"

class SceneBackground : public Scene
{
public:
    SceneBackground();
    ~SceneBackground();

    virtual void Init() override;
    virtual void Render() override;
private:
    std::shared_ptr<GL::ShaderProgram> mBgShaderProgram;

    float viewport_height;
    float viewport_width;
    float focal_length;

    Vector3f origin;
    Vector3f horizontal;
    Vector3f vertical ;
    Vector3f lower_left_corner ;
};