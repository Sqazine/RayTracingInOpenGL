#pragma once
#include <cstdint>
#include <glad/glad.h>

namespace GL
{

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();
        void SetActive(bool isActive);

    private:
        uint32_t m_VertexArrayID;
    };

    inline VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_VertexArrayID);
    }

    inline VertexArray::~VertexArray()
    {
    }

    inline void VertexArray::SetActive(bool isActive)
    {
        if (isActive)
            glBindVertexArray(m_VertexArrayID);
        else
            glBindVertexArray(0);
    }
}