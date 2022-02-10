#pragma once
#include <string>
#include <glad/glad.h>
class GLTexture2D
{
public:
	GLTexture2D() {}
	GLTexture2D(uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t format, uint32_t type, const void *data = NULL,
				uint32_t wrap_s = GL_CLAMP_TO_EDGE, uint32_t wrap_t = GL_CLAMP_TO_EDGE, uint32_t min_filter = GL_NEAREST, uint32_t mag_filter = GL_NEAREST);
	~GLTexture2D();

	void SetActive(int8_t pos);

	int GetTextureWidth() const;
	int GetTextureHeight() const;

	uint32_t m_TextureID;

protected:
	int m_TextureWidth;
	int m_TextureHeight;
};
