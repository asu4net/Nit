#include "RenderCommand.h"
#include "RenderAPI.h"

namespace Nit
{
    void CRenderCommandQueue::ExecuteNext()
    {
        if (m_CommandQueue.empty()) return;
        const auto& command = m_CommandQueue[0];
        //NIT_LOG_TRACE("Execute render command: %s\n", command->GetName());
        command->Execute();
        m_CommandQueue.erase(m_CommandQueue.begin());
    }

    CRenderCommand::CRenderCommand(const TSharedPtr<CRenderAPI>& api)
        : m_RendererAPI(api)
    {
    }
}