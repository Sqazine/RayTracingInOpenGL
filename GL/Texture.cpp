#include "Texture.h"
#include <vector>
namespace GL
{

	Texture2D::Texture2D()
	{
		glGenTextures(1, &m_TextureID);
	}

	Texture2D::Texture2D(Texture2DCreateInfo info)
		: m_Info(info)
	{
		glGenTextures(1, &m_TextureID);
		CreateFrom(info);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void Texture2D::BindTo(uint32_t uniform, uint32_t texIndex)
	{
		glUniform1i(uniform, texIndex);
		glActiveTexture(GL_TEXTURE0 + texIndex);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void Texture2D::UnBindFrom(uint32_t textureIndex)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	uint32_t Texture2D::GetID()
	{
		return m_TextureID;
	}

	void Texture2D::CreateFrom(Texture2DCreateInfo info)
	{
		m_Info = info;
		uint32_t externalFormat = GL_RED, internalFormat = GL_R8, channelType = GL_UNSIGNED_BYTE;
		switch (info.channelMode)
		{
		case ChannelMode::R8:
			externalFormat = GL_RED;
			internalFormat = GL_R8;
			channelType = GL_UNSIGNED_BYTE;
			break;
		case ChannelMode::RG8:
			externalFormat = GL_RG;
			internalFormat = GL_RG8;
			channelType = GL_UNSIGNED_BYTE;
			break;
		case ChannelMode::RGB8:
			externalFormat = GL_RGB;
			internalFormat = GL_RGB8;
			channelType = GL_UNSIGNED_BYTE;
			break;
		case ChannelMode::RGBA8:
			externalFormat = GL_RGBA;
			internalFormat = GL_RGBA8;
			channelType = GL_UNSIGNED_BYTE;
			break;
		case ChannelMode::R32F:
			externalFormat = GL_RED;
			internalFormat = GL_R32F;
			channelType = GL_FLOAT;
			break;
		case ChannelMode::RG32F:
			externalFormat = GL_RG;
			internalFormat = GL_RG32F;
			channelType = GL_FLOAT;
			break;
		case ChannelMode::RGB32F:
			externalFormat = GL_RGB;
			internalFormat = GL_RGB32F;
			channelType = GL_FLOAT;
			break;
		case ChannelMode::RGBA32F:
			externalFormat = GL_RGBA;
			internalFormat = GL_RGBA32F;
			channelType = GL_FLOAT;
			break;
		default:
			break;
		}

		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Info.wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Info.wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Info.minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Info.magFilter);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &m_Info.borderColor.values[0]);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Info.extent.x, m_Info.extent.y, 0, externalFormat, channelType, m_Info.data);

		if (Context::IsSupportExtension("GL_EXT_texture_filter_anisotropic") && m_Info.needAnisotropic)
		{
			float asinoSetting = 0.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &asinoSetting);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, asinoSetting);
		}

		if (m_Info.needMipMap)
			glGenerateTextureMipmap(m_TextureID);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const Texture2DCreateInfo &Texture2D::GetCreateInfo()
	{
		return m_Info;
	}
}