#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <glad/glad.h>

namespace Nit
{
    static GLenum ShaderDataTypeToOpenGlBaseType(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::None:   return 0;
        case ShaderDataType::Float:  return GL_FLOAT;
        case ShaderDataType::Float2: return GL_FLOAT;
        case ShaderDataType::Float3: return GL_FLOAT;
        case ShaderDataType::Float4: return GL_FLOAT;
        case ShaderDataType::Mat3:   return GL_FLOAT;
        case ShaderDataType::Mat4:   return GL_FLOAT;
        case ShaderDataType::Int:    return GL_INT;
        case ShaderDataType::Int2:   return GL_INT;
        case ShaderDataType::Int3:   return GL_INT;
        case ShaderDataType::Int4:   return GL_INT;
        case ShaderDataType::Bool:   return GL_BOOL;
        default:                     return 0;
        }
    }
    
    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        return std::make_shared<VertexArray>();
    }

    VertexArray::VertexArray()
        : m_VertexArrayId(0)
    {
        glCreateVertexArrays(1, &m_VertexArrayId);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_VertexArrayId);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_VertexArrayId);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        assert(!vertexBuffer->GetLayout().GetElements().empty() && "Vertex buffer has no layout!");
        
        glBindVertexArray(m_VertexArrayId);
        vertexBuffer->Bind();
        uint32_t index = 0;
        const BufferLayout& bufferLayout = vertexBuffer->GetLayout();
        for (const auto& element : bufferLayout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGlBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                bufferLayout.GetStride(),
                reinterpret_cast<void*>(static_cast<intptr_t>(element.Offset))
                );
            index++;
        }
        m_VertexBuffers.push_back(vertexBuffer);
    }
    
    void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_VertexArrayId);
        indexBuffer->Bind();
        m_IndexBuffer = indexBuffer;
    }
}
