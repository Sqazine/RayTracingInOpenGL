#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <glad/glad.h>

namespace GL
{

    class IndexBuffer
    {
    public:
        IndexBuffer();
        template <typename T>
        IndexBuffer(const std::vector<T> &input);
        ~IndexBuffer();

        template <typename T>
        void Set(const std::vector<T> &input);

        uint32_t Size() const;

        uint32_t GetDataType() const;

        uint32_t GetID() const;

    private:
        IndexBuffer(const IndexBuffer &) = delete;
        IndexBuffer &operator=(const IndexBuffer &) = delete;

        uint32_t DataStrToGLDataType(std::string_view dataStr);

        uint32_t mIndexBufferID;
        uint32_t mSize;
        uint32_t mDataType;
    };

    template <typename T>
    inline IndexBuffer::IndexBuffer(const std::vector<T> &input)
    {
        glGenBuffers(1, &mIndexBufferID);
        Set(input);
    }

    template <typename T>
    inline void IndexBuffer::Set(const std::vector<T> &input)
    {
        mDataType = DataStrToGLDataType(typeid(T).name());
        mSize = input.size();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mSize * sizeof(T), input.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    inline IndexBuffer::IndexBuffer()
        : mSize(0)
    {
        glGenBuffers(1, &mIndexBufferID);
    }

    inline IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &mIndexBufferID);
    }

    inline uint32_t IndexBuffer::Size() const
    {
        return mSize;
    }

    inline uint32_t IndexBuffer::GetDataType() const
    {
        return mDataType;
    }

    inline uint32_t IndexBuffer::GetID() const
    {
        return mIndexBufferID;
    }

    inline uint32_t IndexBuffer::DataStrToGLDataType(std::string_view dataStr)
    {
        if (dataStr.compare("uint32_t") == 0 || dataStr.compare("unsigned int") == 0)
            return GL_UNSIGNED_INT;
        else if (dataStr.compare("int32_t") == 0 || dataStr.compare("int") == 0)
            return GL_INT;
        else if (dataStr.compare("uint8_t") == 0 || dataStr.compare("unsigned char") == 0)
            return GL_UNSIGNED_BYTE;
        else if (dataStr.compare("int8_t") == 0 || dataStr.compare("signed char") == 0 || dataStr.compare("char") == 0)
            return GL_BYTE;
        else if (dataStr.compare("int16_t") == 0 || dataStr.compare("short") == 0)
            return GL_SHORT;
        else if (dataStr.compare("uint16_t") == 0 || dataStr.compare("unsigned short") == 0)
            return GL_SHORT;
        else if (dataStr.compare("float") == 0)
            return GL_FLOAT;
        return GL_NONE;
    }
}