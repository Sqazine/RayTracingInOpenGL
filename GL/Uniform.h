#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <vector>
#include "../Vector2.h"
#include "../Vector3.h"
#include "../Vector4.h"
namespace GL
{

	template <typename T>
	class Uniform
	{
	public:
		inline static void Set(uint32_t slot, const std::vector<T> &valueArray);
		inline static void Set(uint32_t slot, const T &value);

	private:
		inline static void SetValue(uint32_t slot, T *valueArray, uint32_t arrayLength);
		Uniform() {}
		Uniform(const Uniform &) = delete;
		Uniform &operator=(const Uniform &) = delete;
		~Uniform() {}
	};

#define UNIFORM_ARRAY_IMPL(gl_func, tType, dType)                                                \
	template <>                                                                                  \
	inline void Uniform<tType>::SetValue(uint32_t slot, tType *valueArray, uint32_t arrayLength) \
	{                                                                                            \
		gl_func(slot, (GLsizei)arrayLength, reinterpret_cast<dType *>(&valueArray[0]));          \
	}

#define UNIFORM_MATRIX_ARRAY_IMPL(gl_func, tType, dType)                                          \
	template <>                                                                                   \
	inline void Uniform<tType>::SetValue(uint32_t slot, tType *valueArray, uint32_t arrayLength)  \
	{                                                                                             \
		gl_func(slot, (GLsizei)arrayLength, GL_FALSE, reinterpret_cast<dType *>(&valueArray[0])); \
	}

	UNIFORM_ARRAY_IMPL(glUniform1iv, int32_t, int32_t)
	UNIFORM_ARRAY_IMPL(glUniform1uiv, uint32_t, uint32_t)
	UNIFORM_ARRAY_IMPL(glUniform1fv, float, float)
	UNIFORM_ARRAY_IMPL(glUniform1dv, double, double)

	UNIFORM_ARRAY_IMPL(glUniform2fv, Vector2f, float)
	UNIFORM_ARRAY_IMPL(glUniform2iv, Vector2i32, int32_t)
	UNIFORM_ARRAY_IMPL(glUniform2uiv, Vector2u32, uint32_t)

	UNIFORM_ARRAY_IMPL(glUniform3fv, Vector3f, float)
	UNIFORM_ARRAY_IMPL(glUniform3iv, Vector3i32, int32_t)
	UNIFORM_ARRAY_IMPL(glUniform3uiv, Vector3u32, uint32_t)

	UNIFORM_ARRAY_IMPL(glUniform4fv, Vector4f, float)
	UNIFORM_ARRAY_IMPL(glUniform4iv, Vector4i32, int32_t)
	UNIFORM_ARRAY_IMPL(glUniform4uiv, Vector4u32, uint32_t)

	template <typename T>
	inline void Uniform<T>::Set(uint32_t slot, const std::vector<T> &valueArray)
	{
		SetValue(slot, (T *)&valueArray[0], static_cast<uint32_t>(valueArray.size()));
	}

	template <typename T>
	inline void Uniform<T>::Set(uint32_t slot, const T &value)
	{
		SetValue(slot, (T *)&value, 1);
	}
}