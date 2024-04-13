#include "Command.h"
#include "API.h"

namespace Nit::Render
{
    void CommandQueue::ExecuteNext()
    {
        if (m_CommandQueue.empty()) return;
        const auto& command = m_CommandQueue[0];
        //NIT_LOG_TRACE("Execute render command: %s\n", command->GetName());
        command->Execute();
        m_CommandQueue.erase(m_CommandQueue.begin());
    }

    Command::Command(const SharedPtr<API>& api)
        : m_RendererAPI(api)
    {
    }
}