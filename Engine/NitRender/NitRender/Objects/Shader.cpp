#include "Shader.h"
#include "NitRender/Platform/OpenGL/OpenGLShader.h"

namespace Nit
{
    TSharedPtr<CShader> CShader::Create(EGraphicsAPI api)
    {
        switch (api)
        {
        case EGraphicsAPI::OpenGL:
            return CreateSharedPtr<COpenGlShader>();
        case EGraphicsAPI::None:
        default:
            return nullptr;
        }
    }
}