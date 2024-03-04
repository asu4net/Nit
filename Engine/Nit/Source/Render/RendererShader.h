#pragma once

namespace Nit
{
    class RendererShader
    {
    public:
        static SharedPtr<RendererShader> Create(GraphicsAPI api);

        virtual void Compile(const char* vertexSource, const char* fragmentSource) = 0;

        virtual void SetUniformMat4(const char* uniformName, const Matrix4& mat) const = 0;
        virtual void SetUniformVec4(const char* uniformName, const Vector4& vec) const = 0;
        virtual void SetUniformInt(const char* uniformName, int num) = 0;
        virtual void SetUniformIntArray(const char* uniformName, const int32_t* array, int32_t size) = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };
}