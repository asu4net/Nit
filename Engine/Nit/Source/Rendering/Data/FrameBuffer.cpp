#include "NitPCH.h"
#include "FrameBuffer.h"
#include "glad/glad.h"

namespace Nit
{
    FrameBuffer::FrameBuffer(const Configuration& configuration)
        : m_Configuration(configuration)
    {
        Invalidate();
    }

    FrameBuffer::~FrameBuffer()
    {
        glDeleteFramebuffers(1, &m_FrameBufferId);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void FrameBuffer::Invalidate()
    {
        if (m_FrameBufferId)
        {
            glDeleteFramebuffers(1, &m_FrameBufferId);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthAttachment);
        }
        
        glCreateFramebuffers(1, &m_FrameBufferId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
        
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Configuration.Width, m_Configuration.Height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);
        
        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Configuration.Width, m_Configuration.Height, 0,
            GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            assert(false);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
        glViewport(0, 0, static_cast<GLsizei>(m_Configuration.Width),
            static_cast<GLsizei>(m_Configuration.Height));
    }

    void FrameBuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Resize(const Vec2& size)
    {
        m_Configuration.Width = static_cast<uint32_t>(size.x);
        m_Configuration.Height = static_cast<uint32_t>(size.y);
        Invalidate();
    }
}
