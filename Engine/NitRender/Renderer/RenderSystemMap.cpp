#include "RenderSystemMap.h"

namespace Nit::Render
{
    RenderSystemMap::RenderSystemMap(const APIPtr& api, const FunctionPtr<void()>& nextBatchFn)
        : m_API(api)
        , m_NextBatchFn(nextBatchFn)
    {
    }

    void RenderSystemMap::AddRenderSystem(const String& systemID, IRenderSystem2D* renderSystem2D)
    {
        renderSystem2D->Init(m_API, m_NextBatchFn);
        m_RenderSystems[systemID] = renderSystem2D;
    }
    
    IRenderSystem2D& RenderSystemMap::GetRenderSystem(const String& renderSystemID)
    {
        return *m_RenderSystems[renderSystemID];
    }

    void RenderSystemMap::Each(const FunctionPtr<void(const String&, IRenderSystem2D&)>& eachRenderSystemFn)
    {
        for (auto&[id, renderSystem] : m_RenderSystems)
        {
            eachRenderSystemFn(id, *renderSystem);
        }
    }
}
