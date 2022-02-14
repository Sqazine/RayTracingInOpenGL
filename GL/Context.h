#pragma once
#include <memory>
#include <SDL2/SDL.h>
#include <string_view>
#include "../Vector2.h"
struct WindowCreateInfo
{
    std::string title;
    Vector2u32 extent;
    bool resizeable = true;
};

struct RenderContextCreateInfo
{
    WindowCreateInfo windowCreateInfo;
    bool useVSync = false;
    bool useDoubleBuffer = true;
};

namespace GL
{

class Context
{
public:
    ~Context();

    static void CreateContext(const RenderContextCreateInfo &config);
    static void DestroyContext();

    static void SwapWindow();

    static SDL_GLContext GetContextHandle();

    static SDL_Window *GetWindowHandle();

    static Vector2u32 GetWindowExtent();

    static bool IsSupportExtension(std::string_view extensionName);

private:
    Context();

    static RenderContextCreateInfo m_RenderCreateInfo;

    static SDL_GLContext m_ContextHandle;

    static SDL_Window *m_WindowHandle;
};
}