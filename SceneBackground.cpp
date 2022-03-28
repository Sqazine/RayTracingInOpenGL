#include "SceneBackground.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include "GL/Shader.h"
#include "Utils.h"
#include "Vector3.h"
SceneBackground::SceneBackground()
{
}

SceneBackground::~SceneBackground()
{
}

void SceneBackground::Init()
{
    auto vertShader = GL::ShaderModule(GL::ShaderModuleType::VERTEX, Utils::LoadText(std::string(SHADER_DIR) + "vertex.vert"));
    auto bgFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "background.frag"));

    mBgShaderProgram = std::make_shared<GL::ShaderProgram>();
    mBgShaderProgram->AttachShader(vertShader);
    mBgShaderProgram->AttachShader(bgFragShader);

    auto aspect_ratio=GL::Context::GetWindowExtent().x / (float)GL::Context::GetWindowExtent().y;

    viewport_height=2.0;
    viewport_width=aspect_ratio*viewport_height;
    focal_length=1.0;
    origin=Vector3f::ZERO;
    horizontal=Vector3f(viewport_width,0,0);
    vertical=Vector3f(0.0f,viewport_height,0.0);
    lower_left_corner=origin-horizontal/2.0f-vertical/2.0f-Vector3f(0,0,focal_length);
}

void SceneBackground::Render()
{
    mBgShaderProgram->SetActive(true);

    mBgShaderProgram->SetUniformValue("camera.lower_left_corner", lower_left_corner);
    mBgShaderProgram->SetUniformValue("camera.horizontal", horizontal);
    mBgShaderProgram->SetUniformValue("camera.vertical", vertical);
    mBgShaderProgram->SetUniformValue("camera.origin", origin);

    mScreenSpaceQuad.Bind(mBgShaderProgram->GetAttribute("inPosition"));
    mScreenSpaceQuad.Draw();
    mScreenSpaceQuad.UnBind(mBgShaderProgram->GetAttribute("inPosition"));
    mBgShaderProgram->SetActive(false);
}