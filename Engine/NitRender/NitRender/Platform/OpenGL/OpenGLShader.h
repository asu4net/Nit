#pragma once
#include "NitRender/Objects/Shader.h"

namespace Nit
{
    class COpenGlShader : public CShader
    {
    public:
        COpenGlShader();
        ~COpenGlShader();

        bool Compile(const char* vertexSource, const char* fragmentSource, TString* errorMessage = nullptr) override;

        void SetUniformMat4(const char* uniformName, const CMatrix4& mat) const override;
        void SetUniformVec4(const char* uniformName, const CVector4& vec) const override;
        void SetUniformInt(const char* uniformName, int num) override;
        void SetUniformIntArray(const char* uniformName, const int32_t* array, int32_t size) override;

        void Bind() const override;
        void Unbind() const override;

    private:
        uint32_t m_ShaderId = 0;
        bool m_Compiled = false;
    };
}