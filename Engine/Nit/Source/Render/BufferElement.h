#pragma once
#include "ShaderDataType.h"

namespace Nit
{
    struct BufferElement
    {
        ShaderDataType Type;
        String Name;
        bool Normalized;
        uint32_t Size;
        uint32_t Offset;
        
        BufferElement()
            : Type(ShaderDataType::None)
            , Normalized(false)
            , Size(0)
            , Offset(0)
        {}
        
        BufferElement(const ShaderDataType type, const String& name, const bool normalized = false)
            : Type(type)
            , Name(name)
            , Normalized(normalized)
            , Size(ShaderDataTypeToSize(type))
            , Offset(0)
        {}

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
            case ShaderDataType::None:   return 0;
            case ShaderDataType::Float:  return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat3:   return 3 * 3;
            case ShaderDataType::Mat4:   return 4 * 4;
            case ShaderDataType::Int:    return 1;
            case ShaderDataType::Int2:   return 2;
            case ShaderDataType::Int3:   return 3;
            case ShaderDataType::Int4:   return 4;
            case ShaderDataType::Bool:   return 1;
            default:                     return 0;
            }
        }
    };
}