#pragma once
class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Init() {}
	virtual void ProcessInput() {}
	virtual void Update() {}
	virtual void Render() {}
	virtual void RenderUI(){}
};

