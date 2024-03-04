#pragma once


namespace Nit
{
    class VertexBuffer;
    class IndexBuffer;

    class VertexArray
    {
    public:
        static SharedPtr<VertexArray> Create(GraphicsAPI api);

        virtual ~VertexArray() {};

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer) = 0;

        virtual const DynamicArray<SharedPtr<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const SharedPtr<IndexBuffer>& GetIndexBuffer() const = 0;
    };
}
