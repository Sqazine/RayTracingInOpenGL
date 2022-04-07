#include "SceneMetalMaterial.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include "GL/Shader.h"
#include "Utils.h"
#include "Vector3.h"
#include <imgui.h>
SceneMetalMaterial::SceneMetalMaterial()
    : spp(20), maxDepth(10), mixValue(0.1), mIsFirstFrame(true)
{
}

SceneMetalMaterial::~SceneMetalMaterial()
{
}

void SceneMetalMaterial::Init()
{
    auto vertShader = GL::ShaderModule(GL::ShaderModuleType::VERTEX, Utils::LoadText(std::string(SHADER_DIR) + "vertex.vert"));
    auto metalMaterialFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "metal-material.frag"));
    auto imageBlendEffectFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "image-blend.frag"));
    auto outputFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "empty-post-effect.frag"));

    mMaterialProgram = std::make_shared<GL::ShaderProgram>();
    mMaterialProgram->AttachShader(vertShader);
    mMaterialProgram->AttachShader(metalMaterialFragShader);

    mBlendShaderProgram = std::make_shared<GL::ShaderProgram>();
    mBlendShaderProgram->AttachShader(vertShader);
    mBlendShaderProgram->AttachShader(imageBlendEffectFragShader);

    mOutputShaderProgram = std::make_shared<GL::ShaderProgram>();
    mOutputShaderProgram->AttachShader(vertShader);
    mOutputShaderProgram->AttachShader(outputFragShader);

    Vector3f camPos = Vector3f(0.0f, 0.0f, 1.0f);
    Vector3f camTar = Vector3f(0.0f, 0.0f, -1.0f);
    mCamera = Camera(camPos, camTar, Vector3f(0.0f, 1.0f, 0.0f), 90.0f, GL::Context::GetWindowExtent().x / (float)GL::Context::GetWindowExtent().y, 0.0, (camPos - camTar).Length());

    mWorld.AddSphere(Sphere(Vector3f(0.0f, -100.5f, -1.0f), 100.0f, MATERIAL_LAMBERTIAN, 0));
    mWorld.AddSphere(Sphere(Vector3f(0.0f, 0.0f, -1.0f), 0.5f, MATERIAL_LAMBERTIAN, 1));
    mWorld.AddSphere(Sphere(Vector3f(-1.0f, 0.0f, -1.0f), 0.5f, MATERIAL_METALLIC,0));
    mWorld.AddSphere(Sphere(Vector3f(1.0f, 0.0f, -1.0f), 0.5f, MATERIAL_METALLIC, 1));

    mLambertianMaterials.emplace_back(Lambertian(Vector3f(0.8f, 0.8f, 0.0f)));
    mLambertianMaterials.emplace_back(Lambertian(Vector3f(0.7f, 0.3f, 0.3f)));

    //smooth metal
    // mMetalMaterials.emplace_back(Metal(Vector3f(0.8f)));
    // mMetalMaterials.emplace_back(Metal(Vector3f(0.8f, 0.6f, 0.2f)));

    //fuzzed metal
    mMetalMaterials.emplace_back(Metal(Vector3f(0.8f), 0.3f));
    mMetalMaterials.emplace_back(Metal(Vector3f(0.8f, 0.6f, 0.2f), 1.0f));

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

void SceneMetalMaterial::Render()
{
    // pass:ray tracing
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mPathTracingFBO);
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mMaterialProgram->SetActive(true);

        mMaterialProgram->SetUniformValue("textureExtent", Vector2f(GL::Context::GetWindowExtent().x, GL::Context::GetWindowExtent().y));
        mMaterialProgram->SetUniformValue("rdSeed", Vector4f(Random::GetFloat01(), Random::GetFloat01(), Random::GetFloat01(), Random::GetFloat01()));
        mMaterialProgram->SetUniformValue("maxDepth", maxDepth);

        mMaterialProgram->SetUniformValue("camera.lower_left_corner", mCamera.lower_left_corner);
        mMaterialProgram->SetUniformValue("camera.horizontal", mCamera.horizontal);
        mMaterialProgram->SetUniformValue("camera.vertical", mCamera.vertical);
        mMaterialProgram->SetUniformValue("camera.origin", mCamera.position);
        mMaterialProgram->SetUniformValue("spp", spp);

        for (int i = 0; i < mWorld.spheres.size(); ++i)
        {
            mMaterialProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].origin", mWorld.spheres[i].origin);
            mMaterialProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].radius", mWorld.spheres[i].radius);
             mMaterialProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].materialType", mWorld.spheres[i].materialType);
            mMaterialProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].materialIdx", mWorld.spheres[i].materialIdx);
        }

         for (int i = 0; i < mLambertianMaterials.size(); ++i)
            mMaterialProgram->SetUniformValue("lambertianMaterials[" + std::to_string(i) + "].albedo", mLambertianMaterials[i].albedo);

       
        for (int i = 0; i < mMetalMaterials.size(); ++i)
        {
            mMaterialProgram->SetUniformValue("metallicMaterials[" + std::to_string(i) + "].albedo", mMetalMaterials[i].albedo);
            mMaterialProgram->SetUniformValue("metallicMaterials[" + std::to_string(i) + "].roughness", mMetalMaterials[i].roughness);
        }

        mScreenSpaceQuad.Bind(mMaterialProgram->GetAttribute("inPosition"));
        mScreenSpaceQuad.Draw();
        mScreenSpaceQuad.UnBind(mMaterialProgram->GetAttribute("inPosition"));
        mMaterialProgram->SetActive(false);

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

void SceneMetalMaterial::RenderUI()
{
    ImGui::Begin("Setting");

    ImGui::SliderInt("spp", &spp, 1, 100);
    ImGui::SliderInt("maxDepth", &maxDepth, 1, 100);
    ImGui::SliderFloat("mix value", &mixValue, 0.0, 1.0);

    ImGui::End();
}