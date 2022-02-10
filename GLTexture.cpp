#include "GLTexture.h"
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <string>
#include <vector>


GLTexture2D::GLTexture2D(uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t format, uint32_t type, const void *data, uint32_t wrap_s, uint32_t wrap_t, uint32_t min_filter, uint32_t mag_filter)
	: m_TextureWidth(width), m_TextureHeight(height)
{
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_TextureWidth, m_TextureHeight, 0, format, type, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture2D::~GLTexture2D()
{
	glDeleteTextures(1, &m_TextureID);
}

void GLTexture2D::SetActive(int8_t pos)
{
	if (pos < 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}
	glActiveTexture(GL_TEXTURE0 + pos);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

int GLTexture2D::GetTextureWidth() const
{
	return m_TextureWidth;
}
int GLTexture2D::GetTextureHeight() const
{
	return m_TextureHeight;
}