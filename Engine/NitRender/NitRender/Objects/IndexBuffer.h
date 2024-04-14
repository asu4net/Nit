#pragma once

namespace Nit
{
    class CIndexBuffer
    {
    public:
        static TSharedPtr<CIndexBuffer> Create(EGraphicsAPI api, const uint32_t* indices, uint32_t count);

        virtual ~CIndexBuffer() = default;
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;
    };
}
