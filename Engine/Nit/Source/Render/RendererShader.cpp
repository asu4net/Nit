#include "RendererShader.h"
#include "Platform\OpenGL\OpenGLShader.h"

namespace Nit
{
    SharedPtr<RendererShader> RendererShader::Create(GraphicsAPI api)
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return CreateSharedPtr<OpenGLShader>();
        case GraphicsAPI::None:
        default:
            NIT_CHECK(false, "Invalid API");
            return nullptr;
        }
    }
}