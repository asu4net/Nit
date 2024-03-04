#pragma once
#include "Render/VertexBuffer.h"
#include "Render/BufferLayout.h"

namespace Nit
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:        
        OpenGLVertexBuffer(const void* vertices, uint32_t size);
        OpenGLVertexBuffer(uint32_t size);
        ~OpenGLVertexBuffer() override;
        
        void Bind() const override;
        void Unbind() const override;
        void SetData(const void* data, uint32_t size) const override;
        
        void SetLayout(const BufferLayout& bufferLayout) override;
        const BufferLayout& GetLayout() const override { return m_Layout; }
    
    private:
        uint32_t m_BufferId{};
        BufferLayout m_Layout;
    };
}