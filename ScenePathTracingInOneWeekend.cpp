#include "ScenePathTracingInOneWeekend.h"
#include "Application.h"
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include <imgui.h>
ScenePathTracingInOneWeekend::ScenePathTracingInOneWeekend()
    : m_IsFirstFrame(true), spp(10), depth(10), mixValue(0.1)
{
    m_PostEffectGUIHint="None";
     m_PostEffectGUIHint+='\0';
     m_PostEffectGUIHint+="Sharpen";
     m_PostEffectGUIHint+='\0';
     m_PostEffectGUIHint+="Blur";
     m_PostEffectGUIHint+='\0';
     m_PostEffectGUIHint+="Edge detect";
     m_PostEffectGUIHint+='\0';
}

ScenePathTracingInOneWeekend::~ScenePathTracingInOneWeekend()
{
}

void ScenePathTracingInOneWeekend::Init()
{
    m_PathTracingShader = std::make_shared<GLShader>(std::string(SHADER_DIR) + "ray-tracing-in-one-weekend.vert",std::string(SHADER_DIR) + "ray-tracing-in-one-weekend.frag");
    m_PathTracingShader->Compile();

    m_BlendShader = std::make_shared<GLShader>(std::string(SHADER_DIR) + "image-blend.vert",std::string(SHADER_DIR) + "image-blend.frag");
    m_BlendShader->Compile();

    m_PostEffectShader = std::make_shared<GLShader>(std::string(SHADER_DIR) + "post-effect.vert",std::string(SHADER_DIR) + "post-effect.frag");
    m_PostEffectShader->Compile();

    m_World.AddSphere(Sphere(Vector3f(0.0f, 0.0f, -1.0f), 0.5f, MATERIAL_HALF_LAMBERTIAN, 1));
    m_World.AddSphere(Sphere(Vector3f(-1.1f, 0.0f, -1.0f), 0.5f, MATERIAL_DIELECTRIC, 0));
    m_World.AddSphere(Sphere(Vector3f(-1.1f, 0.0f, -1.0f), -0.45f, MATERIAL_DIELECTRIC, 0));
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
    m_Camera = RayTraceCamera(camPos, camTar, Vector3f(0.0f, 1.0f, 0.0f), 90.0f, GLContext::GetWindowExtent().x / (float)GLContext::GetWindowExtent().y, 0.0, (camPos - camTar).Length());

    m_PathTracingTexture = std::make_shared<GLTexture2D>(GLContext::GetWindowExtent().x, GLContext::GetWindowExtent().y, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE);
    m_PathTracingBlendTexture = std::make_shared<GLTexture2D>(GLContext::GetWindowExtent().x, GLContext::GetWindowExtent().y, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE);

    glGenFramebuffers(1, &m_PathTracingFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_PathTracingFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PathTracingTexture->m_TextureID, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &m_PathTracingBlendFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_PathTracingBlendFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PathTracingBlendTexture->m_TextureID, 0);
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

        m_PathTracingShader->SetActive(true);
        m_PathTracingShader->SetVector2("textureExtent", Vector2f(GLContext::GetWindowExtent().x, GLContext::GetWindowExtent().y));
        m_PathTracingShader->SetVector4("rdSeed", Vector4f(Random::GetFloat01(), Random::GetFloat01(), Random::GetFloat01(), Random::GetFloat01()));

        m_PathTracingShader->SetVector3("camera.lower_left_corner", m_Camera.lower_left_corner);
        m_PathTracingShader->SetVector3("camera.horizontal", m_Camera.horizontal);
        m_PathTracingShader->SetVector3("camera.vertical", m_Camera.vertical);
        m_PathTracingShader->SetVector3("camera.origin", m_Camera.position);
        m_PathTracingShader->SetFloat("camera.lens_radius", m_Camera.lens_radius);
        m_PathTracingShader->SetVector3("camera.front", m_Camera.front);
        m_PathTracingShader->SetVector3("camera.right", m_Camera.right);
        m_PathTracingShader->SetVector3("camera.up", m_Camera.selfUp);

        for (int i = 0; i < m_World.spheres.size(); ++i)
        {
            m_PathTracingShader->SetVector3("world.objects[" + std::to_string(i) + "].origin", m_World.spheres[i].origin);
            m_PathTracingShader->SetFloat("world.objects[" + std::to_string(i) + "].radius", m_World.spheres[i].radius);
            m_PathTracingShader->SetInt("world.objects[" + std::to_string(i) + "].materialType", m_World.spheres[i].materialType);
            m_PathTracingShader->SetInt("world.objects[" + std::to_string(i) + "].materialIdx", m_World.spheres[i].materialIdx);
        }

        for (int i = 0; i < m_LambertianMaterials.size(); ++i)
        {
            m_PathTracingShader->SetVector3("lambertianMaterials[" + std::to_string(i) + "].albedo", m_LambertianMaterials[i].albedo);
        }

        for (int i = 0; i < m_HalfLambertianMaterials.size(); ++i)
        {
            m_PathTracingShader->SetVector3("halfLambertianMaterials[" + std::to_string(i) + "].albedo", m_HalfLambertianMaterials[i].albedo);
        }

        for (int i = 0; i < m_MetalMaterials.size(); ++i)
        {
            m_PathTracingShader->SetVector3("metallicMaterials[" + std::to_string(i) + "].albedo", m_MetalMaterials[i].albedo);
            m_PathTracingShader->SetFloat("metallicMaterials[" + std::to_string(i) + "].roughness", m_MetalMaterials[i].roughness);
        }

        for (int i = 0; i < m_DielectricMaterials.size(); ++i)
        {
            m_PathTracingShader->SetVector3("dielectricMaterials[" + std::to_string(i) + "].albedo", m_DielectricMaterials[i].albedo);
            m_PathTracingShader->SetFloat("dielectricMaterials[" + std::to_string(i) + "].roughness", m_DielectricMaterials[i].roughness);
            m_PathTracingShader->SetFloat("dielectricMaterials[" + std::to_string(i) + "].ior", m_DielectricMaterials[i].ior);
        }

        m_PathTracingShader->SetInt("spp", spp);
        m_PathTracingShader->SetInt("traceDepth", depth);

        m_Quad.Draw();
        m_PathTracingShader->SetActive(false);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    //pass 1
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_PathTracingBlendFBO);

        m_BlendShader->SetActive(true);

        if (m_IsFirstFrame)
        {
            m_PathTracingTexture->SetActive(0);
            m_IsFirstFrame = false;
        }
        else
            m_PathTracingBlendTexture->SetActive(0);

        m_PathTracingTexture->SetActive(1);

        float tmpMixValue;
        if (Input::GetMouse()->GetButtonState(SDL_BUTTON_LEFT) == ButtonState::HOLD)
            tmpMixValue = 1.0f;
        else
            tmpMixValue = mixValue;

        m_BlendShader->SetFloat("mixValue", tmpMixValue);

        m_Quad.Draw();

        m_BlendShader->SetActive(false);

        // glDisable(GL_DEPTH_TEST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    //pass 2
    {
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_PostEffectShader->SetActive(true);
        m_PostEffectShader->SetInt("postEffectType",currentPostEffect);
        m_PathTracingBlendTexture->SetActive(0);

        m_Quad.Draw();

        m_PostEffectShader->SetActive(false);
    }
}

void ScenePathTracingInOneWeekend::RenderUI()
{
    ImGui::Begin("Setting");

    ImGui::Text("FPS:%.1f(%.3fms/frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);

    ImGui::SliderInt("spp", &spp, 1, 20);
    ImGui::SliderInt("depth", &depth, 1, 20);
    ImGui::SliderFloat("mix value", &mixValue, 0.0, 1.0);
    ImGui::Combo("post effect",&currentPostEffect,m_PostEffectGUIHint.c_str());

    ImGui::End();
}