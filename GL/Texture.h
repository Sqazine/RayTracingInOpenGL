#pragma once
#include <cstdint>
#include <string>
#include <glad/glad.h>
#include "Context.h"
#include "../Vector2.h"
#include "../Vector4.h"
namespace GL
{
	enum WrapMode
	{
		REPEAT = GL_REPEAT,
		MIRROR_REPEAT = GL_MIRRORED_REPEAT,
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
	};

	enum FilterMode
	{
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR
	};

	enum ChannelMode
	{
		R8=GL_R8,
		RG8=GL_RG8,
		RGB8=GL_RGB8,
		RGBA8=GL_RGBA8,

		R32F=GL_R32F,
		RG32F=GL_RG32F,
		RGB32F=GL_RGB32F,
		RGBA32F=GL_RGBA32F,
	};

	struct Texture2DCreateInfo
	{
		int32_t wrapS = WrapMode::REPEAT, wrapT = WrapMode::REPEAT;
		int32_t minFilter = FilterMode::LINEAR, magFilter = FilterMode::LINEAR;
		bool needMipMap = false;
		bool needAnisotropic = false;
		Vector4f borderColor=Vector4f::ZERO;
		Vector2u32 extent=Vector2u32::ZERO;
		ChannelMode channelMode= ChannelMode::RGBA8;
		void* data=nullptr;
	};

	class Texture2D
	{
	public:
		Texture2D();
		Texture2D(Texture2DCreateInfo info);
		~Texture2D();

		void CreateFrom(Texture2DCreateInfo info);

		void BindTo(uint32_t uniform, uint32_t texIndex);
		void UnBindFrom(uint32_t textureIndex);

		uint32_t GetID() const;
		const Texture2DCreateInfo& GetCreateInfo();
	protected:

		uint32_t mTextureID;
		Texture2DCreateInfo mInfo;
	};
}