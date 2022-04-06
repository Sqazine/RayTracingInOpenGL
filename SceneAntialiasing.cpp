#include "SceneAntialiasing.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include "GL/Shader.h"
#include "Utils.h"
#include "Vector3.h"
#include <imgui.h>
SceneAntialiasing::SceneAntialiasing()
    : spp(1)
{
}

SceneAntialiasing::~SceneAntialiasing()
{
}

void SceneAntialiasing::Init()
{
    auto vertShader = GL::ShaderModule(GL::ShaderModuleType::VERTEX, Utils::LoadText(std::string(SHADER_DIR) + "vertex.vert"));
    auto antialiasingFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "antialiasing.frag"));

    mAntialiasingShaderProgram = std::make_shared<GL::ShaderProgram>();
    mAntialiasingShaderProgram->AttachShader(vertShader);
    mAntialiasingShaderProgram->AttachShader(antialiasingFragShader);

    mWorld.AddSphere(Sphere(Vector3f(0, 0, -1), 0.5));
    mWorld.AddSphere(Sphere(Vector3f(0, -100.5, -1), 100));

    Vector3f camPos = Vector3f(0.0f, 0.0f, 1.0f);
    Vector3f camTar = Vector3f(0.0f, 0.0f, -1.0f);
    mCamera = Camera(camPos, camTar, Vector3f(0.0f, 1.0f, 0.0f), 90.0f, GL::Context::GetWindowExtent().x / (float)GL::Context::GetWindowExtent().y, 0.0, (camPos - camTar).Length());
}

void SceneAntialiasing::Render()
{
    mAntialiasingShaderProgram->SetActive(true);

    mAntialiasingShaderProgram->SetUniformValue("textureExtent", Vector2f(GL::Context::GetWindowExtent().x, GL::Context::GetWindowExtent().y));
        mAntialiasingShaderProgram->SetUniformValue("rdSeed", Vector4f(Random::GetFloat01(), Random::GetFloat01(), Random::GetFloat01(), Random::GetFloat01()));


    mAntialiasingShaderProgram->SetUniformValue("camera.lower_left_corner", mCamera.lower_left_corner);
    mAntialiasingShaderProgram->SetUniformValue("camera.horizontal", mCamera.horizontal);
    mAntialiasingShaderProgram->SetUniformValue("camera.vertical", mCamera.vertical);
    mAntialiasingShaderProgram->SetUniformValue("camera.origin", mCamera.position);
    mAntialiasingShaderProgram->SetUniformValue("spp", spp);

    for (int i = 0; i < mWorld.spheres.size(); ++i)
    {
        mAntialiasingShaderProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].origin", mWorld.spheres[i].origin);
        mAntialiasingShaderProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].radius", mWorld.spheres[i].radius);
    }

    mScreenSpaceQuad.Bind(mAntialiasingShaderProgram->GetAttribute("inPosition"));
    mScreenSpaceQuad.Draw();
    mScreenSpaceQuad.UnBind(mAntialiasingShaderProgram->GetAttribute("inPosition"));
    mAntialiasingShaderProgram->SetActive(false);
}

void SceneAntialiasing::RenderUI()
{
    ImGui::Begin("Setting");

    ImGui::SliderInt("spp", &spp, 1, 100);

    ImGui::End();
}