#include "SceneDielectricMaterialAndPositionableCamera.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include <imgui.h>
#include "GL/Shader.h"
#include "GL/Renderer.h"
#include "Utils.h"
SceneDielectricMaterialAndPositionableCamera::SceneDielectricMaterialAndPositionableCamera()
    : mIsFirstFrame(true), spp(10), depth(10), mixValue(0.1)
{
    mPostEffectGUIHint = "None";
    mPostEffectGUIHint += '\0';
    mPostEffectGUIHint += "Sharpen";
    mPostEffectGUIHint += '\0';
    mPostEffectGUIHint += "Blur";
    mPostEffectGUIHint += '\0';
    mPostEffectGUIHint += "Edge detect";
    mPostEffectGUIHint += '\0';
}

SceneDielectricMaterialAndPositionableCamera::~SceneDielectricMaterialAndPositionableCamera()
{
}

void SceneDielectricMaterialAndPositionableCamera::Init()
{

    auto vertShader = GL::ShaderModule(GL::ShaderModuleType::VERTEX, Utils::LoadText(std::string(SHADER_DIR) + "vertex.vert"));
    auto rtFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "ray-tracing-in-one-weekend.frag"));
    auto postEffectFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "post-effect.frag"));
    auto imageBlendEffectFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "image-blend.frag"));

    mPathTracingShaderProgram = std::make_shared<GL::ShaderProgram>();
    mPathTracingShaderProgram->AttachShader(vertShader);
    mPathTracingShaderProgram->AttachShader(rtFragShader);

    mBlendShaderProgram = std::make_shared<GL::ShaderProgram>();
    mBlendShaderProgram->AttachShader(vertShader);
    mBlendShaderProgram->AttachShader(imageBlendEffectFragShader);

    mPostEffectShaderProgram = std::make_shared<GL::ShaderProgram>();
    mPostEffectShaderProgram->AttachShader(vertShader);
    mPostEffectShaderProgram->AttachShader(postEffectFragShader);

    mWorld.AddSphere(Sphere(Vector3f(0.0f, 0.0f, -1.0f), 0.5f, MATERIAL_HALF_LAMBERTIAN, 1));
    mWorld.AddSphere(Sphere(Vector3f(-1.1f, 0.0f, -1.0f), 0.5f, MATERIAL_DIELECTRIC, 0));
    mWorld.AddSphere(Sphere(Vector3f(-1.1f, 0.0f, -1.0f), -0.499f, MATERIAL_DIELECTRIC, 0));
    mWorld.AddSphere(Sphere(Vector3f(1.1f, 0.0f, -1.0f), 0.5f, MATERIAL_METALLIC, 1));
    mWorld.AddSphere(Sphere(Vector3f(0.0f, -100.5f, -1.0f), 100.0f, MATERIAL_HALF_LAMBERTIAN, 0));

    mLambertianMaterials.emplace_back(Lambertian(Vector3f(0.8f, 0.8f, 0.0f)));
    mLambertianMaterials.emplace_back(Lambertian(Vector3f(0.1f, 0.2f, 0.5f)));
    mHalfLambertianMaterials.emplace_back(HalfLambertian(Vector3f(0.8f, 0.8f, 0.0f)));
    mHalfLambertianMaterials.emplace_back(HalfLambertian(Vector3f(0.1f, 0.2f, 0.5f)));
    mMetalMaterials.emplace_back(Metal(Vector3f(0.8f), 0.3f));
    mMetalMaterials.emplace_back(Metal(Vector3f(0.8f, 0.6f, 0.2f), 0.0f));
    mDielectricMaterials.emplace_back(Dielectric(1.5f));

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

    mPathTracingFrameBuffer = std::make_shared<GL::FrameBuffer>(mPathTracingTexture.get());
    mPathTracingBlendFrameBuffer = std::make_shared<GL::FrameBuffer>(mPathTracingBlendTexture.get());
}

void SceneDielectricMaterialAndPositionableCamera::ProcessInput()
{
    if (Input::GetMouse()->IsReleativeMode() && Input::GetMouse()->GetButtonState(SDL_BUTTON_LEFT) == ButtonState::HOLD)
        mCamera.ProcessInput();
}

void SceneDielectricMaterialAndPositionableCamera::Update()
{
    if (Input::GetMouse()->GetButtonState(SDL_BUTTON_LEFT) == ButtonState::HOLD)
        mCamera.Update();
}

void SceneDielectricMaterialAndPositionableCamera::Render()
{
    //pass 0
    {
        mPathTracingFrameBuffer->SetActive(true);
        GL::Renderer::ClearColorBuffer(0.2f, 0.3f, 0.5f, 1.0f);

        mPathTracingShaderProgram->SetActive(true);
        mPathTracingShaderProgram->SetUniformValue("textureExtent", Vector2f(GL::Context::GetWindowExtent().x, GL::Context::GetWindowExtent().y));
        mPathTracingShaderProgram->SetUniformValue("rdSeed", Vector4f(Random::GetFloat01(), Random::GetFloat01(), Random::GetFloat01(), Random::GetFloat01()));

        mPathTracingShaderProgram->SetUniformValue("camera.lower_left_corner", mCamera.lower_left_corner);
        mPathTracingShaderProgram->SetUniformValue("camera.horizontal", mCamera.horizontal);
        mPathTracingShaderProgram->SetUniformValue("camera.vertical", mCamera.vertical);
        mPathTracingShaderProgram->SetUniformValue("camera.origin", mCamera.position);
        mPathTracingShaderProgram->SetUniformValue("camera.lensRadius", mCamera.lensRadius);
        mPathTracingShaderProgram->SetUniformValue("camera.right", mCamera.GetLocalRightAxis());
        mPathTracingShaderProgram->SetUniformValue("camera.up", mCamera.GetLocalUpAxis());

        for (int i = 0; i < mWorld.spheres.size(); ++i)
        {
            mPathTracingShaderProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].origin", mWorld.spheres[i].origin);
            mPathTracingShaderProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].radius", mWorld.spheres[i].radius);
            mPathTracingShaderProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].materialType", mWorld.spheres[i].materialType);
            mPathTracingShaderProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].materialIdx", mWorld.spheres[i].materialIdx);
        }
        for (int i = 0; i < mLambertianMaterials.size(); ++i)
            mPathTracingShaderProgram->SetUniformValue("lambertianMaterials[" + std::to_string(i) + "].albedo", mLambertianMaterials[i].albedo);

        for (int i = 0; i < mHalfLambertianMaterials.size(); ++i)
            mPathTracingShaderProgram->SetUniformValue("halfLambertianMaterials[" + std::to_string(i) + "].albedo", mHalfLambertianMaterials[i].albedo);

        for (int i = 0; i < mMetalMaterials.size(); ++i)
        {
            mPathTracingShaderProgram->SetUniformValue("metallicMaterials[" + std::to_string(i) + "].albedo", mMetalMaterials[i].albedo);
            mPathTracingShaderProgram->SetUniformValue("metallicMaterials[" + std::to_string(i) + "].roughness", mMetalMaterials[i].roughness);
        }
        for (int i = 0; i < mDielectricMaterials.size(); ++i)
        {
            mPathTracingShaderProgram->SetUniformValue("dielectricMaterials[" + std::to_string(i) + "].ior", mDielectricMaterials[i].ior);
        }
        mPathTracingShaderProgram->SetUniformValue("spp", spp);
        mPathTracingShaderProgram->SetUniformValue("traceDepth", depth);
        mScreenSpaceQuad.Bind(mPathTracingShaderProgram->GetAttribute("inPosition"));
        mScreenSpaceQuad.Draw();
        mScreenSpaceQuad.UnBind(mPathTracingShaderProgram->GetAttribute("inPosition"));
        mPathTracingShaderProgram->SetActive(false);

        mPathTracingFrameBuffer->SetActive(false);
    }
    //pass 1
    {
        mPathTracingBlendFrameBuffer->SetActive(true);

        mBlendShaderProgram->SetActive(true);

        if (mIsFirstFrame)
        {
            mPathTracingTexture->BindTo(mBlendShaderProgram->GetUniform("oldTexture"), 0);
            mIsFirstFrame = false;
        }
        else
            mPathTracingBlendTexture->BindTo(mBlendShaderProgram->GetUniform("oldTexture"), 0);

        mPathTracingTexture->BindTo(mBlendShaderProgram->GetUniform("newTexture"), 1);

        float tmpMixValue;
        if (Input::GetMouse()->GetButtonState(SDL_BUTTON_LEFT) == ButtonState::HOLD)
            tmpMixValue = 1.0f;
        else
            tmpMixValue = mixValue;

        mBlendShaderProgram->SetUniformValue("mixValue", tmpMixValue);

        mScreenSpaceQuad.Bind(mBlendShaderProgram->GetAttribute("inPosition"));
        mScreenSpaceQuad.Draw();
        mScreenSpaceQuad.UnBind(mBlendShaderProgram->GetAttribute("inPosition"));

        mBlendShaderProgram->SetActive(false);

        mPathTracingBlendFrameBuffer->SetActive(false);
    }
    //pass 2
    {
        GL::Renderer::ClearColorBuffer(0.2f, 0.3f, 0.5f, 1.0f);

        mPostEffectShaderProgram->SetActive(true);
        mPostEffectShaderProgram->SetUniformValue("postEffectType", currentPostEffect);
        mPathTracingBlendTexture->BindTo(mPostEffectShaderProgram->GetUniform("finalTexture"), 0);

        mScreenSpaceQuad.Bind(mPathTracingShaderProgram->GetAttribute("inPosition"));
        mScreenSpaceQuad.Draw();
        mScreenSpaceQuad.UnBind(mPathTracingShaderProgram->GetAttribute("inPosition"));

        mPostEffectShaderProgram->SetActive(false);
    }
}

void SceneDielectricMaterialAndPositionableCamera::RenderUI()
{
    ImGui::Begin("Setting");

    ImGui::SliderInt("spp", &spp, 1, 20);
    ImGui::SliderInt("depth", &depth, 1, 20);
    ImGui::SliderFloat("mix value", &mixValue, 0.0, 1.0);
    ImGui::Combo("post effect", &currentPostEffect, mPostEffectGUIHint.c_str());

    ImGui::End();
}