#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
#include "App.h"
#include "GL/Shader.h"
#include "Shape.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include "World.h"

class SceneAntialiasing : public Scene
{
public:
    SceneAntialiasing();
    ~SceneAntialiasing();

    virtual void Init() override;
    virtual void Render() override;
    virtual void RenderUI() override;

private:
    std::shared_ptr<GL::ShaderProgram> mAntialiasingShaderProgram;

    World mWorld;
    Camera mCamera;

    int32_t spp;
};