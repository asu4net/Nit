#pragma once


namespace Nit
{
    class IndexBuffer
    {
    public:
        static SharedPtr<IndexBuffer> Create(GraphicsAPI api, const uint32_t* indices, uint32_t count);

        virtual ~IndexBuffer() {};
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;
    };
}
