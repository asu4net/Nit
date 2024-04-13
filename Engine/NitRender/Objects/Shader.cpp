#include "Shader.h"
#include "Platform\OpenGL\OpenGLShader.h"

namespace Nit::Render
{
    SharedPtr<Shader> Shader::Create(GraphicsAPI api)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return CreateSharedPtr<OpenGLShader>();
        case GraphicsAPI::None:
        default:
            return nullptr;
        }
    }
}