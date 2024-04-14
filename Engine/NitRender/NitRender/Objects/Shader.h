#pragma once

namespace Nit
{
    class CShader
    {
    public:
        static TSharedPtr<CShader> Create(EGraphicsAPI api);

        virtual bool Compile(const char* vertexSource, const char* fragmentSource, TString* errorMessage = nullptr) = 0;
        
        virtual void SetUniformMat4(const char* uniformName, const CMatrix4& mat) const = 0;
        virtual void SetUniformVec4(const char* uniformName, const CVector4& vec) const = 0;
        virtual void SetUniformInt(const char* uniformName, int num) = 0;
        virtual void SetUniformIntArray(const char* uniformName, const int32_t* array, int32_t size) = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };
    
    using TShaderPtr     = TSharedPtr<CShader>; 
    using TWeakShaderPtr = TWeakPtr<CShader>;
}