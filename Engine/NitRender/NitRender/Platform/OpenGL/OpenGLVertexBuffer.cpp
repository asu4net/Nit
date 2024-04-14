#include "OpenGLVertexBuffer.h"
#include <glad/glad.h>

namespace Nit
{
    COpenGlVertexBuffer::COpenGlVertexBuffer(const void* vertices, const uint32_t size)
        : m_BufferId(0)
    {
        glCreateBuffers(1, &m_BufferId);
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    COpenGlVertexBuffer::COpenGlVertexBuffer(const uint32_t size)
    {
        glCreateBuffers(1, &m_BufferId);
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    COpenGlVertexBuffer::~COpenGlVertexBuffer()
    {
        glDeleteBuffers(1, &m_BufferId);
    }

    void COpenGlVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
    }

    void COpenGlVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void COpenGlVertexBuffer::SetData(const void* data, const uint32_t size) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    void COpenGlVertexBuffer::SetLayout(const CBufferLayout& bufferLayout)
    {
        m_Layout = bufferLayout;
    }
}