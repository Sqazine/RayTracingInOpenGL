#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
#include "App.h"
#include "GL/Shader.h"
#include "GL/Texture.h"
#include "Shape.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include "World.h"
#include "Material.h"

class SceneMetalMaterial : public Scene
{
public:
    SceneMetalMaterial();
    ~SceneMetalMaterial();

    virtual void Init() override;
    virtual void Render() override;
    virtual void RenderUI() override;

private:
    std::shared_ptr<GL::ShaderProgram> mMaterialProgram;
    std::shared_ptr<GL::ShaderProgram> mBlendShaderProgram;
    std::shared_ptr<GL::ShaderProgram> mOutputShaderProgram;

    World mWorld;

     std::vector<Lambertian> mLambertianMaterials;
    std::vector<Metal> mMetalMaterials;

    Camera mCamera;

    uint32_t mPathTracingFBO;
    std::shared_ptr<GL::Texture2D> mPathTracingTexture;

    uint32_t mPathTracingBlendFBO;
    std::shared_ptr<GL::Texture2D> mPathTracingBlendTexture;

    bool mIsFirstFrame;

    int32_t spp;
    int maxDepth;
    float mixValue;

};