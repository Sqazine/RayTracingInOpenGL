#include "SceneMultipleObjects.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include "GL/Shader.h"
#include "Utils.h"
#include "Vector3.h"
SceneMultipleObjects::SceneMultipleObjects()
{
}

SceneMultipleObjects::~SceneMultipleObjects()
{
}

void SceneMultipleObjects::Init()
{
    auto vertShader = GL::ShaderModule(GL::ShaderModuleType::VERTEX, Utils::LoadText(std::string(SHADER_DIR) + "vertex.vert"));
    auto multipleObjectsFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "multiple-objects.frag"));

    mMultipleObjectsShaderProgram = std::make_shared<GL::ShaderProgram>();
    mMultipleObjectsShaderProgram->AttachShader(vertShader);
    mMultipleObjectsShaderProgram->AttachShader(multipleObjectsFragShader);

    auto aspect_ratio = GL::Context::GetWindowExtent().x / (float)GL::Context::GetWindowExtent().y;

    viewport_height = 2.0;
    viewport_width = aspect_ratio * viewport_height;
    focal_length = 1.0;
    origin = Vector3f::ZERO;
    horizontal = Vector3f(viewport_width, 0, 0);
    vertical = Vector3f(0.0f, viewport_height, 0.0);
    lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - Vector3f(0, 0, focal_length);

    mWorld.AddSphere(Sphere(Vector3f(0, 0, -1), 0.5));
    mWorld.AddSphere(Sphere(Vector3f(0, -100.5, -1), 100));
}

void SceneMultipleObjects::Render()
{
    mMultipleObjectsShaderProgram->SetActive(true);

    mMultipleObjectsShaderProgram->SetUniformValue("camera.lower_left_corner", lower_left_corner);
    mMultipleObjectsShaderProgram->SetUniformValue("camera.horizontal", horizontal);
    mMultipleObjectsShaderProgram->SetUniformValue("camera.vertical", vertical);
    mMultipleObjectsShaderProgram->SetUniformValue("camera.origin", origin);

    for (int i = 0; i < mWorld.spheres.size(); ++i)
        {
            mMultipleObjectsShaderProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].origin", mWorld.spheres[i].origin);
            mMultipleObjectsShaderProgram->SetUniformValue("world.objects[" + std::to_string(i) + "].radius", mWorld.spheres[i].radius);
        }

    mScreenSpaceQuad.Bind(mMultipleObjectsShaderProgram->GetAttribute("inPosition"));
    mScreenSpaceQuad.Draw();
    mScreenSpaceQuad.UnBind(mMultipleObjectsShaderProgram->GetAttribute("inPosition"));
    mMultipleObjectsShaderProgram->SetActive(false);
}