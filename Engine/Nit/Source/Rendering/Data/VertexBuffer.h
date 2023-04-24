#pragma once
#include <cstdint>
#include <memory>

#include "BufferLayout.h"

namespace Nit
{
    class VertexBuffer
    {
    public:
        static std::shared_ptr<VertexBuffer> Create(const float* vertices, uint32_t size);
        static std::shared_ptr<VertexBuffer> Create(uint32_t size);
        
        VertexBuffer(const float* vertices, uint32_t size);
        VertexBuffer(uint32_t size);
        ~VertexBuffer();
        
        void Bind() const;
        void Unbind() const;
        void SetData(const void* data, uint32_t size) const;
        
        void SetLayout(const BufferLayout& bufferLayout);
        const BufferLayout& GetLayout() const { return m_Layout; }
    
    private:
        uint32_t m_BufferId{};
        BufferLayout m_Layout;
    };
}