#include "RenderCommandQueue.h"

namespace Nit
{
    void RenderCommandQueue::ExecuteNext()
    {
        if (m_CommandQueue.empty()) return;
        const auto& command = m_CommandQueue[0];
        //printf("%s\n", command->GetName());
        command->Execute();
        m_CommandQueue.erase(m_CommandQueue.begin());
    }
}
