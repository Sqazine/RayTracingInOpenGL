#include "App.h"
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include "ScenePathTracingInOneWeekend.h"
#include "SceneCreatingAnImage.h"
#include "SceneBackground.h"

App::App(RenderContextCreateInfo renderContextCreateInfo)
	: mIsRunning(true), mRenderContextCreateInfo(renderContextCreateInfo)
{
}

App::~App()
{
}

void App::Run()
{
	Init();
	while (mIsRunning)
	{
		Timer::Update(mRenderContextCreateInfo.frameRate);

		ProcessInput();
		Update();
		Draw();
		GL::Context::SwapWindow();
	}
}

void App::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_Log("failed to init sdl!");

	GL::Context::CreateContext(mRenderContextCreateInfo);

	Input::Init();

	Random::Init();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(GL::Context::GetWindowHandle(), GL::Context::GetContextHandle());
	ImGui_ImplOpenGL3_Init("#version 450 core");

	Timer::Init();

	mScenes.emplace_back(std::make_shared<SceneCreatingAnImage>());
	mScenes.emplace_back(std::make_shared<SceneBackground>());
	mScenes.emplace_back(std::make_shared<ScenePathTracingInOneWeekend>());

	for (const auto &scene : mScenes)
		scene->Init();
}

void App::ProcessInput()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
		}
	}
	if (Input::GetKeyboard()->GetKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::PRESS)
		mIsRunning = false;

	Input::ProcessInput(event);

	if (Input::GetKeyboard()->GetKeyState(SDL_SCANCODE_P) == ButtonState::PRESS)
	{
		if (Input::GetMouse()->IsReleativeMode())
			Input::GetMouse()->SetReleativeMode(false);
		else
			Input::GetMouse()->SetReleativeMode(true);
	}
	mScenes[mCurSceneIndex]->ProcessInput();
}

void App::Update()
{
	Input::PreUpdate();

	mScenes[mCurSceneIndex]->Update();

	if (mCurSceneIndex != mPreSceneNum)
	{
		mIsSceneSelectedChanged = true;
		mPreSceneNum = mCurSceneIndex;
	}
	else
		mIsSceneSelectedChanged = false;

	Input::PostUpdate();
}

void App::Draw()
{
	mScenes[mCurSceneIndex]->Render();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(GL::Context::GetWindowHandle());
	ImGui::NewFrame();

	ImGui::Begin("Scene Selector");

	ImGui::Text("Display frequency:%.1fFPS(%.3fms/frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);

	ImGui::RadioButton("SceneCreatingAnImage", &mCurSceneIndex, 0);
	ImGui::RadioButton("SceneBackground", &mCurSceneIndex, 1);
	ImGui::RadioButton("ScenePathTracingInOneWeekend", &mCurSceneIndex, 2);

	ImGui::End();


	mScenes[mCurSceneIndex]->RenderUI();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void App::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	GL::Context::DestroyContext();

	SDL_Quit();
}