#pragma once
#include "Shape.h"
class Scene
{
public:
	Scene() : mScreenSpaceQuad(Mesh(MeshType::QUAD)) {}
	virtual ~Scene() {}

	virtual void Init() {}
	virtual void ProcessInput() {}
	virtual void Update() {}
	virtual void Render() {}
	virtual void RenderUI() {}

protected:
	Mesh mScreenSpaceQuad;
};
