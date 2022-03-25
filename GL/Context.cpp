#include "Context.h"
#include <glad/glad.h>

namespace GL
{

	RenderContextCreateInfo Context::mRenderCreateInfo;

	SDL_GLContext Context::mContextHandle;

	SDL_Window *Context::mWindowHandle = nullptr;

	Context::Context()
	{
	}

	Context::~Context()
	{
	}

	void Context::CreateContext(const RenderContextCreateInfo &config)
	{
		mRenderCreateInfo = config;

		uint32_t windowFlag = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;
		if (config.windowCreateInfo.resizeable)
			windowFlag |= SDL_WINDOW_RESIZABLE;

		mWindowHandle = SDL_CreateWindow(config.windowCreateInfo.title.c_str(),
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
		if (mRenderCreateInfo.useDoubleBuffer)
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		else
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);

		mContextHandle = SDL_GL_CreateContext(mWindowHandle);

		if (!mContextHandle)
			SDL_Log("failed to create SDL opengl context:%s", SDL_GetError());

		if (!gladLoadGL())
			SDL_Log("failed to load GLAD:%s", glGetError());

		//VSync
		int success = SDL_GL_SetSwapInterval((mRenderCreateInfo.useVSync ? 1 : 0));
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
		SDL_GL_DeleteContext(mContextHandle);
	}

	void Context::SwapWindow()
	{
		SDL_GL_SwapWindow(mWindowHandle);
	}

	SDL_GLContext Context::GetContextHandle()
	{
		return mContextHandle;
	}

	SDL_Window *Context::GetWindowHandle()
	{
		return mWindowHandle;
	}

	Vector2u32 Context::GetWindowExtent()
	{
		return mRenderCreateInfo.windowCreateInfo.extent;
	}

	bool Context::IsSupportExtension(std::string_view extensionName)
	{
		return SDL_GL_ExtensionSupported(extensionName.data());
	}
}