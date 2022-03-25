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

	RenderContextCreateInfo mRenderContextCreateInfo;

	bool mIsRunning;

	std::vector<std::shared_ptr<Scene>> mScenes;

	int mCurSceneIndex = 0;
	bool mIsSceneSelectedChanged = false;
	int mPreSceneNum = 0;
};
