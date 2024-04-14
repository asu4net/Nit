#pragma once
#include "NitRender/Objects/IndexBuffer.h"

namespace Nit
{
    class COpenGlIndexBuffer : public CIndexBuffer
    {
    public:
        COpenGlIndexBuffer(const uint32_t* indices, uint32_t count);
        ~COpenGlIndexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        uint32_t GetCount() const override { return m_Count; }

    private:
        uint32_t m_BufferId;
        uint32_t m_Count;
    };
}
