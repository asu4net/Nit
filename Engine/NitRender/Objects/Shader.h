#pragma once

namespace Nit::Render
{
    class Shader
    {
    public:
        static SharedPtr<Shader> Create(GraphicsAPI api);

        virtual bool Compile(const char* vertexSource, const char* fragmentSource, String* errorMessage = nullptr) = 0;
        
        virtual void SetUniformMat4(const char* uniformName, const Matrix4& mat) const = 0;
        virtual void SetUniformVec4(const char* uniformName, const Vector4& vec) const = 0;
        virtual void SetUniformInt(const char* uniformName, int num) = 0;
        virtual void SetUniformIntArray(const char* uniformName, const int32_t* array, int32_t size) = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };
    
    using ShaderPtr     = SharedPtr<Shader>; 
    using WeakShaderPtr = WeakPtr<Shader>;
}