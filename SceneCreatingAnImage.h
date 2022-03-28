#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
#include "App.h"
#include "GL/Shader.h"
#include "Shape.h"
#include <glm/glm.hpp>


class SceneCreatingAnImage : public Scene
{
public:
    SceneCreatingAnImage();
    ~SceneCreatingAnImage();

    virtual void Init() override;
    virtual void Render() override;
private:
    std::shared_ptr<GL::ShaderProgram> mImgShaderProgram;
};