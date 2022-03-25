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
        uint32_t mVertexArrayID;
    };

    inline VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &mVertexArrayID);
    }

    inline VertexArray::~VertexArray()
    {
    }

    inline void VertexArray::SetActive(bool isActive)
    {
        if (isActive)
            glBindVertexArray(mVertexArrayID);
        else
            glBindVertexArray(0);
    }
}