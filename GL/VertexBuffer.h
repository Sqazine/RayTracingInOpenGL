#pragma once
#include <cstdint>
#include <vector>
#include <glad/glad.h>
#include "../Vector2.h"
#include "../Vector3.h"
#include "../Vector4.h"
namespace GL
{
	template <typename T>
	class VertexBuffer
	{
	public:
		VertexBuffer();
		VertexBuffer(const std::vector<T> &inputArray);
		~VertexBuffer();

		void Set(const std::vector<T> &inputArray);
		void BindTo(uint32_t slot);
		void UnBindFrom(uint32_t slot);
		uint32_t Size();

	protected:
		uint32_t m_VertexBufferID;
		uint32_t m_Size;

	private:
		void SetAttribPointer(uint32_t slot);
		VertexBuffer(const VertexBuffer &) = delete;
		VertexBuffer &operator=(const VertexBuffer &other) = delete;
	};

	template <typename T>
	inline VertexBuffer<T>::VertexBuffer()
	{
		glGenBuffers(1, &m_VertexBufferID);
		m_Size = 0;
	}
	template <typename T>
	inline VertexBuffer<T>::VertexBuffer(const std::vector<T> &inputArray)
	{
		glGenBuffers(1, &m_VertexBufferID);
		Set(inputArray);
	}

	template <typename T>
	inline VertexBuffer<T>::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_VertexBufferID);
	}

	template <typename T>
	inline void VertexBuffer<T>::Set(const std::vector<T> &inputArray)
	{
		m_Size = inputArray.size();
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, m_Size * sizeof(T), inputArray.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template <typename T>
	inline void VertexBuffer<T>::BindTo(uint32_t slot)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glEnableVertexAttribArray(slot);
		SetAttribPointer(slot);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template <typename T>
	inline void VertexBuffer<T>::UnBindFrom(uint32_t slot)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glDisableVertexAttribArray(slot);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template <>
	inline void VertexBuffer<int32_t>::SetAttribPointer(uint32_t slot)
	{
		glVertexAttribIPointer(slot, 1, GL_INT, GL_FALSE, reinterpret_cast<void *>(0));
	}

	template <>
	inline void VertexBuffer<float>::SetAttribPointer(uint32_t slot)
	{
		glVertexAttribPointer(slot, 1, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(0));
	}

	template <>
	inline void VertexBuffer<Vector2f>::SetAttribPointer(uint32_t slot)
	{
		glVertexAttribPointer(slot, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(0));
	}

	template <>
	inline void VertexBuffer<Vector3f>::SetAttribPointer(uint32_t slot)
	{
		glVertexAttribPointer(slot, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(0));
	}

	template <>
	inline void VertexBuffer<Vector4f>::SetAttribPointer(uint32_t slot)
	{
		glVertexAttribPointer(slot, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(0));
	}

	template <>
	inline void VertexBuffer<Vector4i32>::SetAttribPointer(uint32_t slot)
	{
		glVertexAttribIPointer(slot, 4, GL_INT, GL_FALSE, reinterpret_cast<void *>(0));
	}

	template <>
	inline void VertexBuffer<Vector4u32>::SetAttribPointer(uint32_t slot)
	{
		glVertexAttribIPointer(slot, 4, GL_UNSIGNED_INT, GL_FALSE, reinterpret_cast<void *>(0));
	}

	template <typename T>
	inline uint32_t VertexBuffer<T>::Size()
	{
		return m_Size;
	}

}