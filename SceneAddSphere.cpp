#include "SceneAddSphere.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include "GL/Shader.h"
#include "Utils.h"
#include "Vector3.h"
SceneAddSphere::SceneAddSphere()
    : mSphere(Sphere(Vector3f(0, 0, -1), 0.5))
{
}

SceneAddSphere::~SceneAddSphere()
{
}

void SceneAddSphere::Init()
{
    auto vertShader = GL::ShaderModule(GL::ShaderModuleType::VERTEX, Utils::LoadText(std::string(SHADER_DIR) + "vertex.vert"));
    auto addSphereFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "add-sphere.frag"));

    mAddSphereShaderProgram = std::make_shared<GL::ShaderProgram>();
    mAddSphereShaderProgram->AttachShader(vertShader);
    mAddSphereShaderProgram->AttachShader(addSphereFragShader);

    auto aspect_ratio = GL::Context::GetWindowExtent().x / (float)GL::Context::GetWindowExtent().y;

    viewport_height = 2.0;
    viewport_width = aspect_ratio * viewport_height;
    focal_length = 1.0;
    origin = Vector3f::ZERO;
    horizontal = Vector3f(viewport_width, 0, 0);
    vertical = Vector3f(0.0f, viewport_height, 0.0);
    lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - Vector3f(0, 0, focal_length);
}

void SceneAddSphere::Render()
{
    mAddSphereShaderProgram->SetActive(true);

    mAddSphereShaderProgram->SetUniformValue("camera.lower_left_corner", lower_left_corner);
    mAddSphereShaderProgram->SetUniformValue("camera.horizontal", horizontal);
    mAddSphereShaderProgram->SetUniformValue("camera.vertical", vertical);
    mAddSphereShaderProgram->SetUniformValue("camera.origin", origin);
    mAddSphereShaderProgram->SetUniformValue("sphere.origin", mSphere.origin);
    mAddSphereShaderProgram->SetUniformValue("sphere.radius", mSphere.radius);

    mScreenSpaceQuad.Bind(mAddSphereShaderProgram->GetAttribute("inPosition"));
    mScreenSpaceQuad.Draw();
    mScreenSpaceQuad.UnBind(mAddSphereShaderProgram->GetAttribute("inPosition"));
    mAddSphereShaderProgram->SetActive(false);
}