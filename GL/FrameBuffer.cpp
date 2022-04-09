#include "FrameBuffer.h"
#include <glad/glad.h>
namespace GL
{
    FrameBuffer::FrameBuffer()
    {
        glGenFramebuffers(1, &mFrameBufferID);
    }

    FrameBuffer::FrameBuffer(const Texture2D *colorAttachment0)
    {
        glGenFramebuffers(1, &mFrameBufferID);
        SetActive(true);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment0->GetID(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::SetActive(bool isActive)
    {
        if (isActive)
            glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
        else
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}