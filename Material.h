#pragma once
#include "Vector3.h"
#define MATERIAL_LAMBERTIAN 0
#define MATERIAL_HALF_LAMBERTIAN 1
#define MATERIAL_METALLIC 2
#define MATERIAL_DIELECTRIC 3
#define MATERIAL_PBR 4

struct Lambertian
{
    Lambertian(Vector3f albedo) : albedo(albedo) {}
    Vector3f albedo;
};

struct HalfLambertian
{
    HalfLambertian(Vector3f albedo) : albedo(albedo) {}
    Vector3f albedo;
};

struct Metal
{
    Metal(Vector3f albedo, float roughness) : albedo(albedo), roughness(roughness) {}
    Vector3f albedo;
    float roughness;
};

struct Dielectric
{
    Dielectric(Vector3f albedo, float roughness, float ior) : albedo(albedo), roughness(roughness), ior(ior) {}
    Vector3f albedo;
    float roughness;
    float ior;
};
