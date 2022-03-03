#pragma once
#include <vector>
#include "Shape.h"
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