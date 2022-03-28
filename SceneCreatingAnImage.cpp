#include "SceneCreatingAnImage.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include <imgui.h>
#include "GL/Shader.h"
#include "Utils.h"
SceneCreatingAnImage::SceneCreatingAnImage()
{
}

SceneCreatingAnImage::~SceneCreatingAnImage()
{
}

void SceneCreatingAnImage::Init()
{
    auto vertShader = GL::ShaderModule(GL::ShaderModuleType::VERTEX, Utils::LoadText(std::string(SHADER_DIR) + "vertex.vert"));
    auto imgFragShader = GL::ShaderModule(GL::ShaderModuleType::FRAGMENT, Utils::LoadText(std::string(SHADER_DIR) + "creating-an-image.frag"));

    mImgShaderProgram = std::make_shared<GL::ShaderProgram>();
    mImgShaderProgram->AttachShader(vertShader);
    mImgShaderProgram->AttachShader(imgFragShader);
}

void SceneCreatingAnImage::Render()
{
    mImgShaderProgram->SetActive(true);
    mScreenSpaceQuad.Bind(mImgShaderProgram->GetAttribute("inPosition"));
    mScreenSpaceQuad.Draw();
    mScreenSpaceQuad.UnBind(mImgShaderProgram->GetAttribute("inPosition"));
    mImgShaderProgram->SetActive(false);
}