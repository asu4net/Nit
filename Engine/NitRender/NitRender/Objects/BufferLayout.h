#pragma once
#include "BufferElement.h"

namespace Nit
{
    class CBufferLayout
    {
    public:
        CBufferLayout() = default;

        CBufferLayout(const std::initializer_list<CBufferElement>& elements)
            : m_Elements(elements)
        {
            uint32_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }
        
        const TDynamicArray<CBufferElement>& GetElements() const { return m_Elements; }
        uint32_t GetStride() const { return m_Stride; }
        
        TDynamicArray<CBufferElement>::iterator begin() { return m_Elements.begin(); }
        TDynamicArray<CBufferElement>::iterator end() { return m_Elements.end(); }
        TDynamicArray<CBufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        TDynamicArray<CBufferElement>::const_iterator end() const { return m_Elements.end(); }
        
    private:
        uint32_t m_Stride = 0;
        TDynamicArray<CBufferElement> m_Elements{};
    };
}