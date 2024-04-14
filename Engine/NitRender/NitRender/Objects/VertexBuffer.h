#pragma once
#include "BufferLayout.h"

namespace Nit
{
    class CVertexBuffer
    {
    public:
        static TSharedPtr<CVertexBuffer> Create(EGraphicsAPI api, const void* vertices, uint32_t size);
        static TSharedPtr<CVertexBuffer> Create(EGraphicsAPI api, uint32_t size);

        virtual ~CVertexBuffer() {};

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void SetData(const void* data, uint32_t size) const = 0;

        virtual void SetLayout(const CBufferLayout& bufferLayout) = 0;
        virtual const CBufferLayout& GetLayout() const = 0;
    };
}