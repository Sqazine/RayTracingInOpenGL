#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
#include "App.h"
#include "GL/Shader.h"
#include "GL/Texture.h"
#include "GL/FrameBuffer.h"
#include "Input.h"
#include "Timer.h"
#include "Material.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include "World.h"


class SceneDielectricMaterialAndPositionableCamera : public Scene
{
public:
    SceneDielectricMaterialAndPositionableCamera();
    ~SceneDielectricMaterialAndPositionableCamera();

    virtual void Init() override;
    virtual void ProcessInput() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void RenderUI() override;

private:
    bool mIsFirstFrame;

    std::shared_ptr<GL::ShaderProgram> mPathTracingShaderProgram;
    std::shared_ptr<GL::ShaderProgram> mBlendShaderProgram;
    std::shared_ptr<GL::ShaderProgram> mPostEffectShaderProgram;

    World mWorld;
    std::vector<Lambertian> mLambertianMaterials;
    std::vector<HalfLambertian> mHalfLambertianMaterials;
    std::vector<Metal> mMetalMaterials;
    std::vector<Dielectric> mDielectricMaterials;
    Camera mCamera;

    std::shared_ptr<GL::FrameBuffer> mPathTracingFrameBuffer;
    std::shared_ptr<GL::Texture2D> mPathTracingTexture;

    std::shared_ptr<GL::FrameBuffer> mPathTracingBlendFrameBuffer;
    std::shared_ptr<GL::Texture2D> mPathTracingBlendTexture;

    std::string mPostEffectGUIHint;


    int32_t spp;
    int32_t depth;
    float mixValue;
    int32_t currentPostEffect=0;
};