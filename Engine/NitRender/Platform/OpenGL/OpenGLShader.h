#pragma once
#include "Objects/Shader.h"

namespace Nit::Render
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader();
        ~OpenGLShader();

        bool Compile(const char* vertexSource, const char* fragmentSource, String* errorMessage = nullptr) override;

        void SetUniformMat4(const char* uniformName, const Matrix4& mat) const override;
        void SetUniformVec4(const char* uniformName, const Vector4& vec) const override;
        void SetUniformInt(const char* uniformName, int num) override;
        void SetUniformIntArray(const char* uniformName, const int32_t* array, int32_t size) override;

        void Bind() const override;
        void Unbind() const override;

    private:
        uint32_t m_ShaderId = 0;
        bool m_Compiled = false;
    };
}