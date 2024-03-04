#include "RenderCommand.h"
#include "RendererAPI.h"

namespace Nit
{
    void RenderCommandQueue::ExecuteNext()
    {
        if (m_CommandQueue.empty()) return;
        const auto& command = m_CommandQueue[0];
        //NIT_LOG_TRACE("Execute render command: %s\n", command->GetName());
        command->Execute();
        m_CommandQueue.erase(m_CommandQueue.begin());
    }

    RenderCommand::RenderCommand(const SharedPtr<RendererAPI>& api)
        : m_RendererAPI(api)
    {
    }
}