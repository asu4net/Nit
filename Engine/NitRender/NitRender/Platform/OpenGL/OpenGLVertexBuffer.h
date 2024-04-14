#pragma once
#include "NitRender/Objects/VertexBuffer.h"
#include "NitRender/Objects/BufferLayout.h"

namespace Nit
{
    class COpenGlVertexBuffer : public CVertexBuffer
    {
    public:        
        COpenGlVertexBuffer(const void* vertices, uint32_t size);
        COpenGlVertexBuffer(uint32_t size);
        ~COpenGlVertexBuffer() override;
        
        void Bind() const override;
        void Unbind() const override;
        void SetData(const void* data, uint32_t size) const override;
        
        void SetLayout(const CBufferLayout& bufferLayout) override;
        const CBufferLayout& GetLayout() const override { return m_Layout; }
    
    private:
        uint32_t m_BufferId = 0;
        CBufferLayout m_Layout;
    };
}