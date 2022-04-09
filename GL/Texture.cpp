#include "Texture.h"
#include <vector>
namespace GL
{

	Texture2D::Texture2D()
	{
		glGenTextures(1, &mTextureID);
	}

	Texture2D::Texture2D(Texture2DCreateInfo info)
		: mInfo(info)
	{
		glGenTextures(1, &mTextureID);
		CreateFrom(info);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &mTextureID);
	}

	void Texture2D::BindTo(uint32_t uniform, uint32_t texIndex)
	{
		glUniform1i(uniform, texIndex);
		glActiveTexture(GL_TEXTURE0 + texIndex);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
	}

	void Texture2D::UnBindFrom(uint32_t textureIndex)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	uint32_t Texture2D::GetID() const
	{
		return mTextureID;
	}

	void Texture2D::CreateFrom(Texture2DCreateInfo info)
	{
		mInfo = info;
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

		glBindTexture(GL_TEXTURE_2D, mTextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mInfo.wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mInfo.wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mInfo.minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mInfo.magFilter);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &mInfo.borderColor.values[0]);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mInfo.extent.x, mInfo.extent.y, 0, externalFormat, channelType, mInfo.data);

		if (Context::IsSupportExtension("GL_EXT_texture_filter_anisotropic") && mInfo.needAnisotropic)
		{
			float asinoSetting = 0.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &asinoSetting);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, asinoSetting);
		}

		if (mInfo.needMipMap)
			glGenerateTextureMipmap(mTextureID);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const Texture2DCreateInfo &Texture2D::GetCreateInfo()
	{
		return mInfo;
	}
}