#pragma once
#include <memory>
#include "ImGuiWidget.h"
#include "Core/Delegates/Delegate.h"
#include "Core/Utility/Singleton.h"

namespace Nit
{
    class Window;
    
    class ImGuiRenderer : public Singleton<ImGuiRenderer>
    {
    public:
        ImGuiConfigFlags ConfigFlags;
        
        ImGuiRenderer();
        ImGuiRenderer(ImGuiRenderer&& other) = delete;

        static ImGuiRenderer& CreateAndInitialize(const std::shared_ptr<Window>& window, bool bSetDefaultConfiguration = true, const Delegate<void()>& customConfiguration = {});
        static void FinalizeAndDestroy();
        
        template<typename T, typename ...TArgs>
        std::shared_ptr<T> CreateRootWidget(TArgs&& ...args)
        {
            if (m_RootWidget)
                DestroyRootWidget();
            std::shared_ptr<T> widget = std::make_shared<T>(std::forward<TArgs>(args)...);
            m_RootWidget = widget;
            widget->Create();
            return widget;
        }

        void DestroyRootWidget();
        
        void Initialize(const std::shared_ptr<Window>& window, bool bSetDefaultConfiguration = true, Delegate<void()> customConfiguration = {});
        void DrawWidgets();
        void Finalize();
        
    private:
        std::weak_ptr<Window> m_Window;
        std::shared_ptr<ImGuiWidget> m_RootWidget;

        void ConfigureFlags();
        void ConfigureStyle();
    };
}
