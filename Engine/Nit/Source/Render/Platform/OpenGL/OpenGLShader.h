#pragma once
#include "Render/RendererShader.h"

namespace Nit
{
    class OpenGLShader : public RendererShader
    {
    public:
        OpenGLShader();
        ~OpenGLShader();

        void Compile(const char* vertexSource, const char* fragmentSource) override;

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