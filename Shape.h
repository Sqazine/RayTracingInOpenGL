#pragma once
#include "Vector3.h"

struct Shape
{
    int materialType;
    int materialIdx;
};

struct Sphere : public Shape
{
    Sphere(Vector3f o, float r, int matType, int matIdx) : origin(o), radius(r)
    {
        materialType = matType;
        materialIdx = matIdx;
    }
    
    Vector3f origin;
    float radius;
};