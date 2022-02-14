#include "Application.h"
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include "Input.h"
#include "Timer.h"
#include "Random.h"
#include "ScenePathTracingInOneWeekend.h"

Application::Application(RenderContextCreateInfo renderContextCreateInfo)
	: m_IsRunning(true), m_RenderContextCreateInfo(renderContextCreateInfo)
{
}

Application::~Application()
{
}

void Application::Run()
{
	Init();
	while (m_IsRunning)
	{
		Timer::Update(60);

		ProcessInput();
		Update();
		Draw();
		GL::Context::SwapWindow();
	}
}

void Application::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_Log("failed to init sdl!");

	GL::Context::CreateContext(m_RenderContextCreateInfo);

	Input::Init();

	Random::Init();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(GL::Context::GetWindowHandle(), GL::Context::GetContextHandle());
	ImGui_ImplOpenGL3_Init("#version 450 core");

	Timer::Init();

	m_Scenes.emplace_back(std::make_shared<ScenePathTracingInOneWeekend>());

	for (const auto &scene : m_Scenes)
		scene->Init();
}

void Application::ProcessInput()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_IsRunning = false;
		}
	}
	if (Input::GetKeyboard()->GetKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::PRESS)
		m_IsRunning = false;

	Input::ProcessInput(event);

	if (Input::GetKeyboard()->GetKeyState(SDL_SCANCODE_P) == ButtonState::PRESS)
	{
		if (Input::GetMouse()->IsReleativeMode())
			Input::GetMouse()->SetReleativeMode(false);
		else
			Input::GetMouse()->SetReleativeMode(true);
	}
	m_Scenes[m_CurSceneIndex]->ProcessInput();
}

void Application::Update()
{
	Input::PreUpdate();

	m_Scenes[m_CurSceneIndex]->Update();

	if (m_CurSceneIndex != m_PreSceneNum)
	{
		m_IsSceneSelectedChanged = true;
		m_PreSceneNum = m_CurSceneIndex;
	}
	else
		m_IsSceneSelectedChanged = false;

	Input::PostUpdate();
}

void Application::Draw()
{
	m_Scenes[m_CurSceneIndex]->Render();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(GL::Context::GetWindowHandle());
	ImGui::NewFrame();

	m_Scenes[m_CurSceneIndex]->RenderUI();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	GL::Context::DestroyContext();

	SDL_Quit();
}