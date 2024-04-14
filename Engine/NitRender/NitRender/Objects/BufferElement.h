#pragma once
#include "ShaderDataType.h"

namespace Nit
{
    struct CBufferElement
    {
        EShaderDataType Type;
        TString Name = "";
        bool Normalized;
        uint32_t Size = 0;
        uint32_t Offset = 0;
        
        CBufferElement()
            : Type(EShaderDataType::None)
            , Normalized(false)
        {}
        
        CBufferElement(const EShaderDataType type, const TString& name, const bool normalized = false)
            : Type(type)
            , Name(name)
            , Normalized(normalized)
            , Size(ShaderDataTypeToSize(type))
        {}

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
            case EShaderDataType::None:   return 0;
            case EShaderDataType::Float:  return 1;
            case EShaderDataType::Float2: return 2;
            case EShaderDataType::Float3: return 3;
            case EShaderDataType::Float4: return 4;
            case EShaderDataType::Mat3:   return 3 * 3;
            case EShaderDataType::Mat4:   return 4 * 4;
            case EShaderDataType::Int:    return 1;
            case EShaderDataType::Int2:   return 2;
            case EShaderDataType::Int3:   return 3;
            case EShaderDataType::Int4:   return 4;
            case EShaderDataType::Bool:   return 1;
            default:                     return 0;
            }
        }
    };
}