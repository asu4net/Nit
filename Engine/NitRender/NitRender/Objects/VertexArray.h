#pragma once

namespace Nit
{
    class CVertexBuffer;
    class CIndexBuffer;

    class CVertexArray
    {
    public:
        static TSharedPtr<CVertexArray> Create(EGraphicsAPI api);

        virtual ~CVertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const TSharedPtr<CVertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(const TSharedPtr<CIndexBuffer>& indexBuffer) = 0;

        virtual const TDynamicArray<TSharedPtr<CVertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const TSharedPtr<CIndexBuffer>& GetIndexBuffer() const = 0;
    };
}
