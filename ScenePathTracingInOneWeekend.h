#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
#include "Application.h"
#include "GL/Shader.h"
#include "Input.h"
#include "Timer.h"
#include "GL/Texture2D.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include <glm/glm.hpp>

struct Sphere
{
    Sphere(Vector3f o, float r, int matType, int matIdx) : origin(o), radius(r), materialType(matType), materialIdx(matIdx) {}
    Vector3f origin;
    float radius;
    int materialType;
    int materialIdx;
};

struct World
{
    World() {}
    World(const std::vector<Sphere> spheres) : spheres(spheres) {}
    void AddSphere(Sphere sphere)
    {
        spheres.emplace_back(sphere);
    }
    std::vector<Sphere> spheres;
};



class ScenePathTracingInOneWeekend : public Scene
{
public:
    ScenePathTracingInOneWeekend();
    ~ScenePathTracingInOneWeekend();

    virtual void Init() override;
    virtual void ProcessInput() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void RenderUI() override;

private:
    bool m_IsFirstFrame;

    std::shared_ptr<GL::ShaderProgram> m_PathTracingShaderProgram;
    std::shared_ptr<GL::ShaderProgram> m_BlendShaderProgram;
    std::shared_ptr<GL::ShaderProgram> m_PostEffectShaderProgram;

    Mesh m_Quad;

    World m_World;
    std::vector<Lambertian> m_LambertianMaterials;
    std::vector<HalfLambertian> m_HalfLambertianMaterials;
    std::vector<Metal> m_MetalMaterials;
    std::vector<Dielectric> m_DielectricMaterials;
    Camera m_Camera;

    uint32_t m_PathTracingFBO;
    std::shared_ptr<GL::Texture2D> m_PathTracingTexture;

    uint32_t m_PathTracingBlendFBO;
    std::shared_ptr<GL::Texture2D> m_PathTracingBlendTexture;

    std::string m_PostEffectGUIHint;


    int32_t spp;
    int32_t depth;
    float mixValue;
    int32_t currentPostEffect=0;
};