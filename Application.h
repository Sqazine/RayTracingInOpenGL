#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include "Scene.h"
#include "GLContext.h"
#include "Vector2.h"

class Application
{
public:
	Application(RenderContextCreateInfo renderContextCreateInfo);
	~Application();

	void Run();

private:
	void Init();
	void ProcessInput();
	void Update();
	void Draw();
	void CleanUp();

	RenderContextCreateInfo m_RenderContextCreateInfo;

	bool m_IsRunning;

	std::vector<std::shared_ptr<Scene>> m_Scenes;

	int m_CurSceneIndex = 0;
	bool m_IsSceneSelectedChanged = false;
	int m_PreSceneNum = 0;
};
