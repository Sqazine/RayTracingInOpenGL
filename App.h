#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include "Scene.h"
#include "GL/Context.h"
#include "Vector2.h"

class App
{
public:
	App(RenderContextCreateInfo renderContextCreateInfo);
	~App();

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
