#include "IndexBuffer.h"
#include <glad/glad.h>

namespace Nit
{
    std::shared_ptr<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, const uint32_t count)
    {
        return std::make_shared<IndexBuffer>(indices, count);
    }

    IndexBuffer::IndexBuffer(const uint32_t* indices, const uint32_t count)
        : m_BufferId(0)
        , m_Count(count)
    {
        glCreateBuffers(1, &m_BufferId);
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_BufferId);
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
    }

    void IndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
