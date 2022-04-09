#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
#include "App.h"
#include "GL/Shader.h"
#include "GL/Texture.h"
#include "GL/FrameBuffer.h"
#include "Shape.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include "World.h"

class SceneDiffuseMaterial : public Scene
{
public:
    SceneDiffuseMaterial();
    ~SceneDiffuseMaterial();

    virtual void Init() override;
    virtual void Render() override;
    virtual void RenderUI() override;

private:
    std::shared_ptr<GL::ShaderProgram> mDiffuseMaterialProgram;
    std::shared_ptr<GL::ShaderProgram> mBlendShaderProgram;
       std::shared_ptr<GL::ShaderProgram> mOutputShaderProgram;

    World mWorld;
    Camera mCamera;

    std::shared_ptr<GL::FrameBuffer> mPathTracingFrameBuffer;
    std::shared_ptr<GL::Texture2D> mPathTracingTexture;

    std::shared_ptr<GL::FrameBuffer> mPathTracingBlendFrameBuffer;
    std::shared_ptr<GL::Texture2D> mPathTracingBlendTexture;

    bool mIsFirstFrame;


    int32_t spp;
    int curMaterialType;
    int maxDepth;
    float mixValue;

    std::string curMaterialTypeGUIHint;
};