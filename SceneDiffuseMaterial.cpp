#include "SceneDiffuseMaterial.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include "GL/Shader.h"
#include "Utils.h"
#include "Vector3.h"
#include <imgui.h>
SceneDiffuseMaterial::SceneDiffuseMaterial()
    : spp(20), maxDepth(10), curMaterialType(0), mixValue(0.1), mIsFirstFrame(true)
{
    curMaterialTypeGUIHint = "Simple Diffuse";
    curMaterialTypeGUIHint += '\0';
    curMaterialTypeGUIHint += "Gamma Corrected Simple Diffuse";
    curMaterialTypeGUIHint += '\0';
    curMaterialTypeGUIHint += "Lambertian";
    curMaterialTypeGUIHint += '\0';
    curMaterialTypeGUIHint += "HalfLambertian";
    curMaterialTypeGUIHint += '\0';
}

SceneDiffuseMaterial::~SceneDiffuseMaterial()
{
}

void SceneDiffuseMaterial::Init()
{
    auto vertShader = GL::ShaderModule(GL::ShaderModuleType::VERTEX, Utils::LoadText(std::string(SHADER_DIR) + "vertex.vert"));
    auto diffuseMaterialFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "diffuse-material.frag"));
    auto imageBlendEffectFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "image-blend.frag"));
    auto outputFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "empty-post-effect.frag"));

    mDiffuseMaterialProgram = std::make_shared<GL::ShaderProgram>();
    mDiffuseMaterialProgram->AttachShader(vertShader);
    mDiffuseMaterialProgram->AttachShader(diffuseMaterialFragShader);

    mBlendShaderProgram = std::make_shared<GL::ShaderProgram>();
    mBlendShaderProgram->AttachShader(vertShader);
    mBlendShaderProgram->AttachShader(imageBlendEffectFragShader);

    mOutputShaderProgram = std::make_shared<GL::ShaderProgram>();
    mOutputShaderProgram->AttachShader(vertShader);
    mOutputShaderProgram->AttachShader(outputFragShader);

    mWorld.AddSphere(Sphere(Vector3f(0, 0, -1), 0.5));
    mWorld.AddSphere(Sphere(Vector3f(0, -100.5, -1), 100));

    Vector3f camPos = Vector3f(0.0f, 0.0f, 1.0f);
    Vector3f camTar = Vector3f(0.0f, 0.0f, -1.0f);
    mCamera = Camera(camPos, camTar, Vector3f(0.0f, 1.0f, 0.0f), 90.0f, GL::Context::GetWindowExtent().x / (float)GL::Context::GetWindowExtent().y, 0.0, (camPos - camTar).Length());

    GL::Texture2DCreateInfo textureCreateInfo = {};
    textureCreateInfo.wrapS = GL::WrapMode::CLAMP_TO_EDGE;
    textureCreateInfo.wrapT = GL::WrapMode::CLAMP_TO_EDGE;
    textureCreateInfo.needMipMap = false;
    textureCreateInfo.needAnisotropic = false;
    textureCreateInfo.minFilter = GL::FilterMode::NEAREST;
    textureCreateInfo.magFilter = GL::FilterMode::NEAREST;
    textureCreateInfo.extent = GL::Context::GetWindowExtent();
    textureCreateInfo.channelMode = GL::ChannelMode::RGB8,
    textureCreateInfo.borderColor = Vector4f::ZERO;
    textureCreateInfo.data = nullptr;

    mPathTracingTexture = std::make_shared<GL::Texture2D>(textureCreateInfo);
    mPathTracingBlendTexture = std::make_shared<GL::Texture2D>(textureCreateInfo);

    glGenFramebuffers(1, &mPathTracingFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mPathTracingFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPathTracingTexture->GetID(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &mPathTracingBlendFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mPathTracingBlendFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPathTracingBlendTexture->GetID(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneDiffuseMaterial::Render()
{
    // pass:ray tracing
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mPathTracingFBO);
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mDiffuseMaterialProgram->SetActive(true);

        mDiffuseMaterialProgram->SetUniformValue("textureExtent", Vector2f(GL::Context::GetWindowExtent().x, GL::Context::GetWindowExtent().y));
        mDiffuseMaterialProgram->SetUniformValue("rdSeed", Vector4f(Random::GetFloat01(), Random::GetFloat01(), Random::GetFloat01(), Random::GetFloat01()));
        mDiffuseMaterialProgram->SetUniformValue("maxDepth", maxDepth);
        mDiffuseMaterialProgram->SetUniformValue("curMaterialType", curMaterialType);

        mDiffuseMaterialProgram->SetUniformValue("camera.lower_left_corner", mCamera.lower_left_corner);
        mDiffuseMaterialProgram->SetUniformValue("camera.horizontal", mCamera.horizontal);
        mDiffuseMaterialProgram->SetUniformValue("camera.vertical", mCamera.vertical);
        mDiffuseMaterialProgram->SetUniformValue("camera.origin", mCamera.position);
        mDiffuseMaterialProgram->SetUniformValue("spp", spp);

        for (int i = 0; i < mWorld.spheres.size(); ++i)
        {
            mDiffuseMaterialProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].origin", mWorld.spheres[i].origin);
            mDiffuseMaterialProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].radius", mWorld.spheres[i].radius);
        }

        mScreenSpaceQuad.Bind(mDiffuseMaterialProgram->GetAttribute("inPosition"));
        mScreenSpaceQuad.Draw();
        mScreenSpaceQuad.UnBind(mDiffuseMaterialProgram->GetAttribute("inPosition"));
        mDiffuseMaterialProgram->SetActive(false);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // pass :history frame accumulation
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mPathTracingBlendFBO);
        mBlendShaderProgram->SetActive(true);

        if (mIsFirstFrame)
        {
            mPathTracingTexture->BindTo(mBlendShaderProgram->GetUniform("oldTexture"), 0);
            mIsFirstFrame = false;
        }
        else
            mPathTracingBlendTexture->BindTo(mBlendShaderProgram->GetUniform("oldTexture"), 0);

        mPathTracingTexture->BindTo(mBlendShaderProgram->GetUniform("newTexture"), 1);

        mBlendShaderProgram->SetUniformValue("mixValue", mixValue);

        mScreenSpaceQuad.Bind(mBlendShaderProgram->GetAttribute("inPosition"));
        mScreenSpaceQuad.Draw();
        mScreenSpaceQuad.UnBind(mBlendShaderProgram->GetAttribute("inPosition"));

        mBlendShaderProgram->SetActive(false);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // pass :final output
    {
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mOutputShaderProgram->SetActive(true);
        mPathTracingBlendTexture->BindTo(mOutputShaderProgram->GetUniform("finalTexture"), 0);

        mScreenSpaceQuad.Bind(mOutputShaderProgram->GetAttribute("inPosition"));
        mScreenSpaceQuad.Draw();
        mScreenSpaceQuad.UnBind(mOutputShaderProgram->GetAttribute("inPosition"));

        mOutputShaderProgram->SetActive(false);
    }
}

void SceneDiffuseMaterial::RenderUI()
{
    ImGui::Begin("Setting");

    ImGui::SliderInt("spp", &spp, 1, 100);
    ImGui::SliderInt("maxDepth", &maxDepth, 1, 100);
    ImGui::SliderFloat("mix value", &mixValue, 0.0, 1.0);
    ImGui::Combo("material type", &curMaterialType, curMaterialTypeGUIHint.c_str());

    ImGui::End();
}