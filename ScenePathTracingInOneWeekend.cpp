#include "ScenePathTracingInOneWeekend.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include <imgui.h>
#include "GL/Shader.h"
#include "Utils.h"
ScenePathTracingInOneWeekend::ScenePathTracingInOneWeekend()
    : m_IsFirstFrame(true), spp(10), depth(10), mixValue(0.1)
{
    m_PostEffectGUIHint = "None";
    m_PostEffectGUIHint += '\0';
    m_PostEffectGUIHint += "Sharpen";
    m_PostEffectGUIHint += '\0';
    m_PostEffectGUIHint += "Blur";
    m_PostEffectGUIHint += '\0';
    m_PostEffectGUIHint += "Edge detect";
    m_PostEffectGUIHint += '\0';
}

ScenePathTracingInOneWeekend::~ScenePathTracingInOneWeekend()
{
}

void ScenePathTracingInOneWeekend::Init()
{
    m_Quad = Mesh(MeshType::QUAD);

    auto vertShader = GL::ShaderModule(GL::ShaderModuleType::VERTEX, Utils::LoadText(std::string(SHADER_DIR) + "vertex.vert"));
    auto rtFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "ray-tracing-in-one-weekend.frag"));
    auto postEffectFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "post-effect.frag"));
    auto imageBlendEffectFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "image-blend.frag"));

    m_PathTracingShaderProgram = std::make_shared<GL::ShaderProgram>();
    m_PathTracingShaderProgram->AttachShader(vertShader);
    m_PathTracingShaderProgram->AttachShader(rtFragShader);

    m_BlendShaderProgram = std::make_shared<GL::ShaderProgram>();
    m_BlendShaderProgram->AttachShader(vertShader);
    m_BlendShaderProgram->AttachShader(imageBlendEffectFragShader);

    m_PostEffectShaderProgram = std::make_shared<GL::ShaderProgram>();
    m_PostEffectShaderProgram->AttachShader(vertShader);
    m_PostEffectShaderProgram->AttachShader(postEffectFragShader);

    m_World.AddSphere(Sphere(Vector3f(0.0f, 0.0f, -1.0f), 0.5f, MATERIAL_HALF_LAMBERTIAN, 1));
    m_World.AddSphere(Sphere(Vector3f(-1.1f, 0.0f, -1.0f), 0.5f, MATERIAL_DIELECTRIC, 0));
    m_World.AddSphere(Sphere(Vector3f(-1.1f, 0.0f, -1.0f), -0.499f, MATERIAL_DIELECTRIC, 0));
    m_World.AddSphere(Sphere(Vector3f(1.1f, 0.0f, -1.0f), 0.5f, MATERIAL_METALLIC, 1));
    m_World.AddSphere(Sphere(Vector3f(0.0f, -100.5f, -1.0f), 100.0f, MATERIAL_HALF_LAMBERTIAN, 0));

    m_LambertianMaterials.emplace_back(Lambertian(Vector3f(0.8f, 0.8f, 0.0f)));
    m_LambertianMaterials.emplace_back(Lambertian(Vector3f(0.1f, 0.2f, 0.5f)));
    m_HalfLambertianMaterials.emplace_back(HalfLambertian(Vector3f(0.8f, 0.8f, 0.0f)));
    m_HalfLambertianMaterials.emplace_back(HalfLambertian(Vector3f(0.1f, 0.2f, 0.5f)));
    m_MetalMaterials.emplace_back(Metal(Vector3f(0.8f), 0.3f));
    m_MetalMaterials.emplace_back(Metal(Vector3f(0.8f, 0.6f, 0.2f), 0.0f));
    m_DielectricMaterials.emplace_back(Dielectric(Vector3f(1.0f), 0.1f, 1.5f));

    Vector3f camPos = Vector3f(0.0f, 0.0f, 1.0f);
    Vector3f camTar = Vector3f(0.0f, 0.0f, -1.0f);
    m_Camera = Camera(camPos, camTar, Vector3f(0.0f, 1.0f, 0.0f), 90.0f, GL::Context::GetWindowExtent().x / (float)GL::Context::GetWindowExtent().y, 0.0, (camPos - camTar).Length());

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

    m_PathTracingTexture = std::make_shared<GL::Texture2D>(textureCreateInfo);
    m_PathTracingBlendTexture = std::make_shared<GL::Texture2D>(textureCreateInfo);

    glGenFramebuffers(1, &m_PathTracingFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_PathTracingFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PathTracingTexture->GetID(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &m_PathTracingBlendFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_PathTracingBlendFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PathTracingBlendTexture->GetID(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ScenePathTracingInOneWeekend::ProcessInput()
{
    if (Input::GetMouse()->IsReleativeMode() && Input::GetMouse()->GetButtonState(SDL_BUTTON_LEFT) == ButtonState::HOLD)
        m_Camera.ProcessInput();
}

void ScenePathTracingInOneWeekend::Update()
{
    if (Input::GetMouse()->GetButtonState(SDL_BUTTON_LEFT) == ButtonState::HOLD)
        m_Camera.Update();
}

void ScenePathTracingInOneWeekend::Render()
{
    //pass 0
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_PathTracingFBO);
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_PathTracingShaderProgram->SetActive(true);
        m_PathTracingShaderProgram->SetUniformValue("textureExtent", Vector2f(GL::Context::GetWindowExtent().x, GL::Context::GetWindowExtent().y));
        m_PathTracingShaderProgram->SetUniformValue("rdSeed", Vector4f(Random::GetFloat01(), Random::GetFloat01(), Random::GetFloat01(), Random::GetFloat01()));

        m_PathTracingShaderProgram->SetUniformValue("camera.lower_left_corner", m_Camera.lower_left_corner);
        m_PathTracingShaderProgram->SetUniformValue("camera.horizontal", m_Camera.horizontal);
        m_PathTracingShaderProgram->SetUniformValue("camera.vertical", m_Camera.vertical);
        m_PathTracingShaderProgram->SetUniformValue("camera.origin", m_Camera.position);
        m_PathTracingShaderProgram->SetUniformValue("camera.lensRadius", m_Camera.lensRadius);
        m_PathTracingShaderProgram->SetUniformValue("camera.right", m_Camera.GetLocalRightAxis());
        m_PathTracingShaderProgram->SetUniformValue("camera.up", m_Camera.GetLocalUpAxis());

        for (int i = 0; i < m_World.spheres.size(); ++i)
        {
            m_PathTracingShaderProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].origin", m_World.spheres[i].origin);
            m_PathTracingShaderProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].radius", m_World.spheres[i].radius);
            m_PathTracingShaderProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].materialType", m_World.spheres[i].materialType);
            m_PathTracingShaderProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].materialIdx", m_World.spheres[i].materialIdx);
        }
        for (int i = 0; i < m_LambertianMaterials.size(); ++i)
            m_PathTracingShaderProgram->SetUniformValue("lambertianMaterials[" + std::to_string(i) + "].albedo", m_LambertianMaterials[i].albedo);

        for (int i = 0; i < m_HalfLambertianMaterials.size(); ++i)
            m_PathTracingShaderProgram->SetUniformValue("halfLambertianMaterials[" + std::to_string(i) + "].albedo", m_HalfLambertianMaterials[i].albedo);

        for (int i = 0; i < m_MetalMaterials.size(); ++i)
        {
            m_PathTracingShaderProgram->SetUniformValue("metallicMaterials[" + std::to_string(i) + "].albedo", m_MetalMaterials[i].albedo);
            m_PathTracingShaderProgram->SetUniformValue("metallicMaterials[" + std::to_string(i) + "].roughness", m_MetalMaterials[i].roughness);
        }
        for (int i = 0; i < m_DielectricMaterials.size(); ++i)
        {
            m_PathTracingShaderProgram->SetUniformValue("dielectricMaterials[" + std::to_string(i) + "].albedo", m_DielectricMaterials[i].albedo);
            m_PathTracingShaderProgram->SetUniformValue("dielectricMaterials[" + std::to_string(i) + "].roughness", m_DielectricMaterials[i].roughness);
            m_PathTracingShaderProgram->SetUniformValue("dielectricMaterials[" + std::to_string(i) + "].ior", m_DielectricMaterials[i].ior);
        }
        m_PathTracingShaderProgram->SetUniformValue("spp", spp);
        m_PathTracingShaderProgram->SetUniformValue("traceDepth", depth);
        m_Quad.Bind(m_PathTracingShaderProgram->GetAttribute("inPosition"));
        m_Quad.Draw();
        m_Quad.UnBind(m_PathTracingShaderProgram->GetAttribute("inPosition"));
        m_PathTracingShaderProgram->SetActive(false);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    //pass 1
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_PathTracingBlendFBO);

        m_BlendShaderProgram->SetActive(true);

        if (m_IsFirstFrame)
        {
            m_PathTracingTexture->BindTo(m_BlendShaderProgram->GetUniform("oldTexture"), 0);
            m_IsFirstFrame = false;
        }
        else
            m_PathTracingBlendTexture->BindTo(m_BlendShaderProgram->GetUniform("oldTexture"), 0);

        m_PathTracingTexture->BindTo(m_BlendShaderProgram->GetUniform("newTexture"), 1);

        float tmpMixValue;
        if (Input::GetMouse()->GetButtonState(SDL_BUTTON_LEFT) == ButtonState::HOLD)
            tmpMixValue = 1.0f;
        else
            tmpMixValue = mixValue;

        m_BlendShaderProgram->SetUniformValue("mixValue", tmpMixValue);

        m_Quad.Bind(m_PathTracingShaderProgram->GetAttribute("inPosition"));
        m_Quad.Draw();
        m_Quad.UnBind(m_PathTracingShaderProgram->GetAttribute("inPosition"));

        m_BlendShaderProgram->SetActive(false);

        // glDisable(GL_DEPTH_TEST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    //pass 2
    {
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_PostEffectShaderProgram->SetActive(true);
        m_PostEffectShaderProgram->SetUniformValue("postEffectType", currentPostEffect);
        m_PathTracingBlendTexture->BindTo(m_PostEffectShaderProgram->GetUniform("finalTexture"), 0);

        m_Quad.Bind(m_PathTracingShaderProgram->GetAttribute("inPosition"));
        m_Quad.Draw();
        m_Quad.UnBind(m_PathTracingShaderProgram->GetAttribute("inPosition"));

        m_PostEffectShaderProgram->SetActive(false);
    }
}

void ScenePathTracingInOneWeekend::RenderUI()
{
    ImGui::Begin("Setting");

    ImGui::Text("FPS:%.1f(%.3fms/frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);

    ImGui::SliderInt("spp", &spp, 1, 20);
    ImGui::SliderInt("depth", &depth, 1, 20);
    ImGui::SliderFloat("mix value", &mixValue, 0.0, 1.0);
    ImGui::Combo("post effect", &currentPostEffect, m_PostEffectGUIHint.c_str());

    ImGui::End();
}