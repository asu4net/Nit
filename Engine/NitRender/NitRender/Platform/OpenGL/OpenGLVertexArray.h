#pragma once
#include "NitRender/Objects/VertexArray.h"

namespace Nit
{
    class CVertexBuffer;
    class CIndexBuffer;
    
    class COpenGlVertexArray : public CVertexArray
    {
    public:        
        COpenGlVertexArray();
        ~COpenGlVertexArray() override;
        
        void Bind() const override;
        void Unbind() const override;
        
        void AddVertexBuffer(const TSharedPtr<CVertexBuffer>& vertexBuffer) override;
        void SetIndexBuffer(const TSharedPtr<CIndexBuffer>& indexBuffer) override;

        const TDynamicArray<TSharedPtr<CVertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
        const TSharedPtr<CIndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
        
    private:
        uint32_t m_VertexArrayId;
        TDynamicArray<TSharedPtr<CVertexBuffer>> m_VertexBuffers;
        TSharedPtr<CIndexBuffer> m_IndexBuffer;
    };
}
