#include "OpenGLVertexArray.h"
#include "NitRender/Objects/VertexBuffer.h"
#include "NitRender/Objects/IndexBuffer.h"
#include <glad/glad.h>

namespace Nit
{
    static GLenum ShaderDataTypeToOpenGlBaseType(const EShaderDataType type)
    {
        switch (type)
        {
        case EShaderDataType::None:   return 0;
        case EShaderDataType::Float:  return GL_FLOAT;
        case EShaderDataType::Float2: return GL_FLOAT;
        case EShaderDataType::Float3: return GL_FLOAT;
        case EShaderDataType::Float4: return GL_FLOAT;
        case EShaderDataType::Mat3:   return GL_FLOAT;
        case EShaderDataType::Mat4:   return GL_FLOAT;
        case EShaderDataType::Int:    return GL_INT;
        case EShaderDataType::Int2:   return GL_INT;
        case EShaderDataType::Int3:   return GL_INT;
        case EShaderDataType::Int4:   return GL_INT;
        case EShaderDataType::Bool:   return GL_BOOL;
        default:                     return 0;
        }
    }

    COpenGlVertexArray::COpenGlVertexArray()
        : m_VertexArrayId(0)
    {
        glCreateVertexArrays(1, &m_VertexArrayId);
    }

    COpenGlVertexArray::~COpenGlVertexArray()
    {
        glDeleteVertexArrays(1, &m_VertexArrayId);
    }

    void COpenGlVertexArray::Bind() const
    {
        glBindVertexArray(m_VertexArrayId);
    }

    void COpenGlVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void COpenGlVertexArray::AddVertexBuffer(const TSharedPtr<CVertexBuffer>& vertexBuffer)
    {
        assert(!vertexBuffer->GetLayout().GetElements().empty() && "Vertex buffer has no layout!");
        
        glBindVertexArray(m_VertexArrayId);
        vertexBuffer->Bind();
        uint32_t index = 0;
        const CBufferLayout& bufferLayout = vertexBuffer->GetLayout();
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
    
    void COpenGlVertexArray::SetIndexBuffer(const TSharedPtr<CIndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_VertexArrayId);
        indexBuffer->Bind();
        m_IndexBuffer = indexBuffer;
    }
}