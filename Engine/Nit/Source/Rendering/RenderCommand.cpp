#include "RenderCommand.h"
#include "RendererAPI.h"

namespace Nit
{
    RenderCommand::RenderCommand()
    {
        m_RendererAPI.reset(RendererAPI::Create());
    }
}