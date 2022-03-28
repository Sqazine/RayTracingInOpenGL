#include "SceneSurfaceNormal.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include "GL/Shader.h"
#include "Utils.h"
#include "Vector3.h"
SceneSurfaceNormal::SceneSurfaceNormal()
    : mSphere(Sphere(Vector3f(0, 0, -1), 0.5))
{
}

SceneSurfaceNormal::~SceneSurfaceNormal()
{
}

void SceneSurfaceNormal::Init()
{
    auto vertShader = GL::ShaderModule(GL::ShaderModuleType::VERTEX, Utils::LoadText(std::string(SHADER_DIR) + "vertex.vert"));
    auto surfaceNormalFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "surface-normal.frag"));

    mSurfaceNormalShaderProgram = std::make_shared<GL::ShaderProgram>();
    mSurfaceNormalShaderProgram->AttachShader(vertShader);
    mSurfaceNormalShaderProgram->AttachShader(surfaceNormalFragShader);

    auto aspect_ratio = GL::Context::GetWindowExtent().x / (float)GL::Context::GetWindowExtent().y;

    viewport_height = 2.0;
    viewport_width = aspect_ratio * viewport_height;
    focal_length = 1.0;
    origin = Vector3f::ZERO;
    horizontal = Vector3f(viewport_width, 0, 0);
    vertical = Vector3f(0.0f, viewport_height, 0.0);
    lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - Vector3f(0, 0, focal_length);
}

void SceneSurfaceNormal::Render()
{
    mSurfaceNormalShaderProgram->SetActive(true);

    mSurfaceNormalShaderProgram->SetUniformValue("camera.lower_left_corner", lower_left_corner);
    mSurfaceNormalShaderProgram->SetUniformValue("camera.horizontal", horizontal);
    mSurfaceNormalShaderProgram->SetUniformValue("camera.vertical", vertical);
    mSurfaceNormalShaderProgram->SetUniformValue("camera.origin", origin);
    mSurfaceNormalShaderProgram->SetUniformValue("sphere.origin", mSphere.origin);
    mSurfaceNormalShaderProgram->SetUniformValue("sphere.radius", mSphere.radius);

    mScreenSpaceQuad.Bind(mSurfaceNormalShaderProgram->GetAttribute("inPosition"));
    mScreenSpaceQuad.Draw();
    mScreenSpaceQuad.UnBind(mSurfaceNormalShaderProgram->GetAttribute("inPosition"));
    mSurfaceNormalShaderProgram->SetActive(false);
}