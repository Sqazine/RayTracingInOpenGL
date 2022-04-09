#pragma once
#include <cstdint>
#include "Texture.h"
namespace GL
{
    class FrameBuffer
    {
    public:
        FrameBuffer();
        FrameBuffer(const Texture2D* colorAttachment0);

        void SetActive(bool isActive);
    private:
        uint32_t mFrameBufferID;
    };
}