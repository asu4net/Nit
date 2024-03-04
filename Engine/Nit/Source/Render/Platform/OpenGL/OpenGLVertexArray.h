#pragma once
#include "Render/VertexArray.h"

namespace Nit
{
    class VertexBuffer;
    class IndexBuffer;
    
    class OpenGLVertexArray : public VertexArray
    {
    public:        
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;
        
        void Bind() const override;
        void Unbind() const override;
        
        void AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer) override;
        void SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer) override;

        const DynamicArray<SharedPtr<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
        const SharedPtr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
        
    private:
        uint32_t m_VertexArrayId;
        DynamicArray<SharedPtr<VertexBuffer>> m_VertexBuffers;
        SharedPtr<IndexBuffer> m_IndexBuffer;
    };
}
