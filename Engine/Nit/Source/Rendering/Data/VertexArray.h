#pragma once

namespace Nit
{
    class VertexBuffer;
    class IndexBuffer;
    
    class VertexArray
    {
    public:
        static Shared<VertexArray> Create();
        
        VertexArray();
        ~VertexArray();
        
        void Bind() const;
        void Unbind() const;
        
        void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer);
        void SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer);

        const std::vector<Shared<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
        const Shared<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
        
    private:
        uint32_t m_VertexArrayId;
        std::vector<Shared<VertexBuffer>> m_VertexBuffers;
        Shared<IndexBuffer> m_IndexBuffer;
    };
}
