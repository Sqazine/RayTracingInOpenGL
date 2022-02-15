#include "App.h"

#undef main

// force use discrete GPU
#ifdef __cplusplus
extern "C"
{
#endif
	_declspec(dllexport) unsigned long NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#ifdef __cplusplus
}
#endif

int main(int argc, char **argv)
{
	WindowCreateInfo windowInfo={};
	windowInfo.title="RayTracingInOpenGL";
	windowInfo.extent=Vector2u32(1024,768);
	windowInfo.resizeable=false;

	RenderContextCreateInfo renderInfo{};
	renderInfo.windowCreateInfo=windowInfo;

	auto app = std::make_unique<App>(renderInfo);

	app->Run();
	return 0;
}