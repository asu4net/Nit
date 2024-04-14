#pragma once

namespace Nit
{
    enum class EShaderDataType
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static uint32_t ShaderDataTypeToSize(const EShaderDataType type)
    {
        switch (type)
        {
        case EShaderDataType::None:      return 0; 
        case EShaderDataType::Float:     return 4;
        case EShaderDataType::Float2:    return 4 * 2;
        case EShaderDataType::Float3:    return 4 * 3;
        case EShaderDataType::Float4:    return 4 * 4;
        case EShaderDataType::Mat3:      return 4 * 3 * 3;
        case EShaderDataType::Mat4:      return 4 * 4 * 4;
        case EShaderDataType::Int:       return 4;
        case EShaderDataType::Int2:      return 4 * 2;
        case EShaderDataType::Int3:      return 4 * 3;
        case EShaderDataType::Int4:      return 4 * 4;
        case EShaderDataType::Bool:      return 1;
        default:                        return 0;
        }
    }
}
