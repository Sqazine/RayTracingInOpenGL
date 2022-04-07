#include "SceneRayTracingInOneWeekend.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include <imgui.h>
#include "GL/Shader.h"
#include "Utils.h"
SceneRayTracingInOneWeekend::SceneRayTracingInOneWeekend()
    : mIsFirstFrame(true), spp(10), depth(10), mixValue(0.1),camAperture(0.1f),camVFov(20.0f)
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

SceneRayTracingInOneWeekend::~SceneRayTracingInOneWeekend()
{
}

void SceneRayTracingInOneWeekend::Init()
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

    InitScene();

    Vector3f camPos = Vector3f(13.0f, 2.0f, 3.0f);
    Vector3f camTar = Vector3f(0.0f, 0.0f, 0.0f);

    camFocusDist=10.0;

    mCamera = Camera(camPos, camTar, Vector3f(0.0f, 1.0f, 0.0f), camVFov, GL::Context::GetWindowExtent().x / (float)GL::Context::GetWindowExtent().y, camAperture,camFocusDist );

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

void SceneRayTracingInOneWeekend::ProcessInput()
{
    if (Input::GetMouse()->IsReleativeMode() && Input::GetMouse()->GetButtonState(SDL_BUTTON_LEFT) == ButtonState::HOLD)
        mCamera.ProcessInput();
}

void SceneRayTracingInOneWeekend::Update()
{
    mCamera.lensRadius=camAperture/2.0f;
    mCamera.vfov=camVFov;
    mCamera.focusDistance=camFocusDist;
    mCamera.target=mCamera.position+mCamera.GetLocalFrontAxis()*camFocusDist;
    if (Input::GetMouse()->GetButtonState(SDL_BUTTON_LEFT) == ButtonState::HOLD)
        mCamera.Update();
}

void SceneRayTracingInOneWeekend::Render()
{
    //pass 0
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mPathTracingFBO);
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    //pass 1
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

        // glDisable(GL_DEPTH_TEST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    //pass 2
    {
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mPostEffectShaderProgram->SetActive(true);
        mPostEffectShaderProgram->SetUniformValue("postEffectType", currentPostEffect);
        mPathTracingBlendTexture->BindTo(mPostEffectShaderProgram->GetUniform("finalTexture"), 0);

        mScreenSpaceQuad.Bind(mPathTracingShaderProgram->GetAttribute("inPosition"));
        mScreenSpaceQuad.Draw();
        mScreenSpaceQuad.UnBind(mPathTracingShaderProgram->GetAttribute("inPosition"));

        mPostEffectShaderProgram->SetActive(false);
    }
}

void SceneRayTracingInOneWeekend::RenderUI()
{
    ImGui::Begin("Setting");

    ImGui::SliderInt("spp", &spp, 1, 20);
    ImGui::SliderInt("depth", &depth, 1, 20);
    ImGui::SliderFloat("mix value", &mixValue, 0.0, 1.0);
    ImGui::SliderFloat("camera aperture", &camAperture, 0.0, 5.0);
    ImGui::SliderFloat("camera vfov", &camVFov, 0.0, 180);
    ImGui::SliderFloat("camera focus distance", &camFocusDist, 1.0, 10);
    ImGui::Combo("post effect", &currentPostEffect, mPostEffectGUIHint.c_str());

    ImGui::End();
}

void SceneRayTracingInOneWeekend::InitScene()
{
    mLambertianMaterials.emplace_back(Vector3f(0.5f, 0.5f, 0.5f));
    mWorld.AddSphere(Sphere(Vector3f(0.0f, -1000.0f, -1.0f), 1000.0f, MATERIAL_LAMBERTIAN, mLambertianMaterials.size()-1));

    for(int32_t a=-3;a<4;++a)
    {
        for(int32_t b=-3;b<3;++b)
        {
            auto choose_mat=Random::GetFloat01();
            Vector3f center=Vector3f(a+0.9f*Random::GetFloat01(),0.2,b+0.9f*Random::GetFloat01());

            if((center-Vector3f(4,0.2,0)).Length()>0.9f)
            {
                if(choose_mat<0.8)
                {
                    //lambertian
                    auto albedo=Random::GetVector3<float>(0.0,1.0)*Random::GetVector3<float>(0.0,1.0);
                    mLambertianMaterials.emplace_back(albedo);
                    mWorld.AddSphere(Sphere(center,0.2,MATERIAL_LAMBERTIAN,mLambertianMaterials.size()-1));
                }
                else if(choose_mat<0.95)
                {
                    //metal
                     auto albedo=Random::GetVector3<float>(0.5,1.0);
                     auto fuzzed=Random::GetFloat(0.0,0.5);
                     mMetalMaterials.emplace_back(albedo,fuzzed);
                     mWorld.AddSphere(Sphere(center,0.2,MATERIAL_METALLIC,mMetalMaterials.size()-1));
                }
                else
                {
                    //glass
                    mDielectricMaterials.emplace_back(1.5);
                    mWorld.AddSphere(Sphere(center,0.2,MATERIAL_DIELECTRIC,mDielectricMaterials.size()-1));
                }
            }
        }
    }

    mDielectricMaterials.emplace_back(1.5);
    mWorld.AddSphere(Sphere(Vector3f(0,1,0),1.0,MATERIAL_DIELECTRIC,mDielectricMaterials.size()-1));

    mLambertianMaterials.emplace_back(Vector3f(0.4,0.2,0.1));
    mWorld.AddSphere(Sphere(Vector3f(-4,1,0),1.0,MATERIAL_LAMBERTIAN,mLambertianMaterials.size()-1));

    mMetalMaterials.emplace_back(Vector3f(0.7,0.6,0.5));
    mWorld.AddSphere(Sphere(Vector3f(4,1,0),1.0,MATERIAL_METALLIC,mMetalMaterials.size()-1));
}