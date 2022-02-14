#include "Context.h"
#include <glad/glad.h>

namespace GL
{

	RenderContextCreateInfo Context::m_RenderCreateInfo;

	SDL_GLContext Context::m_ContextHandle;

	SDL_Window *Context::m_WindowHandle = nullptr;

	Context::Context()
	{
	}

	Context::~Context()
	{
	}

	void Context::CreateContext(const RenderContextCreateInfo &config)
	{
		m_RenderCreateInfo = config;

		uint32_t windowFlag = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;
		if (config.windowCreateInfo.resizeable)
			windowFlag |= SDL_WINDOW_RESIZABLE;

		m_WindowHandle = SDL_CreateWindow(config.windowCreateInfo.title.c_str(),
										  SDL_WINDOWPOS_CENTERED,
										  SDL_WINDOWPOS_CENTERED,
										  config.windowCreateInfo.extent.x,
										  config.windowCreateInfo.extent.y,
										  windowFlag);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		if (m_RenderCreateInfo.useDoubleBuffer)
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		else
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);

		m_ContextHandle = SDL_GL_CreateContext(m_WindowHandle);

		if (!m_ContextHandle)
			SDL_Log("failed to create SDL opengl context:%s", SDL_GetError());

		if (!gladLoadGL())
			SDL_Log("failed to load GLAD:%s", glGetError());

		//VSync
		int success = SDL_GL_SetSwapInterval((m_RenderCreateInfo.useVSync ? 1 : 0));
		if (success == -1)
			SDL_Log("failed to open/close opengl VSync:%s", SDL_GetError());
		{
			SDL_Log("OpenGL Info:\n");
			SDL_Log("Vendor:%s", glGetString(GL_VENDOR));
			SDL_Log("Renderer:%s", glGetString(GL_RENDERER));
			SDL_Log("Version:%s", glGetString(GL_VERSION));
			SDL_Log("Extensions:%s", glGetString(GL_EXTENSIONS));
		}
	}

	void Context::DestroyContext()
	{
		SDL_GL_DeleteContext(m_ContextHandle);
	}

	void Context::SwapWindow()
	{
		SDL_GL_SwapWindow(m_WindowHandle);
	}

	SDL_GLContext Context::GetContextHandle()
	{
		return m_ContextHandle;
	}

	SDL_Window *Context::GetWindowHandle()
	{
		return m_WindowHandle;
	}

	Vector2u32 Context::GetWindowExtent()
	{
		return m_RenderCreateInfo.windowCreateInfo.extent;
	}

	bool Context::IsSupportExtension(std::string_view extensionName)
	{
		return SDL_GL_ExtensionSupported(extensionName.data());
	}
}