#pragma once
#include "RenderSystem2D.h"

namespace Nit::Render
{
    class RenderSystemMap
    {
    public:
        RenderSystemMap(const APIPtr& api, const FunctionPtr<void()>& nextBatchFn);

        void AddRenderSystem(const String& systemID, IRenderSystem2D* renderSystem2D);
        IRenderSystem2D& GetRenderSystem(const String& renderSystemID);
        
        template<typename T>
        void CreateRenderSystem()
        {
            T* system = new T();
            AddRenderSystem(system->GetPrimitiveID(), static_cast<IRenderSystem2D*>(system));
        }

        template<typename T>
        T& GetRenderSystem(const String& renderSystemID)
        {
            return static_cast<T&>(GetRenderSystem(renderSystemID));
        }

        void Each(const FunctionPtr<void(const String&, IRenderSystem2D&)>& eachRenderSystemFn);
        
    private:
        APIPtr m_API = nullptr;
        FunctionPtr<void()> m_NextBatchFn;
        Map<String, IRenderSystem2D*> m_RenderSystems;
    };
}