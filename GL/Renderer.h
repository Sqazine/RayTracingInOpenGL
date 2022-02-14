#pragma once
#include <cstdint>
#include <memory>
#include "IndexBuffer.h"
#include <glad/glad.h>

namespace GL
{

	enum RenderType
	{
		POINTS = GL_POINTS,
		LINES = GL_LINES,
		LINE_STRIP = GL_LINE_STRIP,
		LINE_LOOP = GL_LINE_LOOP,
		TRIANGLES = GL_TRIANGLES,
		TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		TRIANGLE_FAN = GL_TRIANGLE_FAN,
	};

	class Renderer
	{
	public:
		void ClearColorBuffer(float r, float g, float b, float a);

		void ClearDepthBuffer();
		void ClearStencilBuffer();

		void EnableDepthTest(bool active);
		void EnableCullFace(bool active);

		void Render(const std::shared_ptr<IndexBuffer> &ibo, RenderType mode);
		void Render(uint32_t vertexCount, RenderType mode);
		void RenderInstanced(const std::shared_ptr<IndexBuffer> &ibo, RenderType mode, uint32_t instanceCount);
		void RenderInstanced(uint32_t vertexCount, RenderType mode, uint32_t instanceCount);
	};

}