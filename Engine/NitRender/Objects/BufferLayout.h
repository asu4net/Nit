#pragma once
#include "BufferElement.h"

namespace Nit::Render
{
    class BufferLayout
    {
    public:
        BufferLayout()
            : m_Stride(0)
        {}
        
        BufferLayout(const std::initializer_list<BufferElement>& elements)
            : m_Stride(0)
            , m_Elements(elements)
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
        
        const DynamicArray<BufferElement>& GetElements() const { return m_Elements; }
        uint32_t GetStride() const { return m_Stride; }
        
        DynamicArray<BufferElement>::iterator begin() { return m_Elements.begin(); }
        DynamicArray<BufferElement>::iterator end() { return m_Elements.end(); }
        DynamicArray<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        DynamicArray<BufferElement>::const_iterator end() const { return m_Elements.end(); }
        
    private:
        uint32_t m_Stride;
        DynamicArray<BufferElement> m_Elements;
    };
}