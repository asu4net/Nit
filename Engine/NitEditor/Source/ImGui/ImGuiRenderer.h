#pragma once
#include "ImGuiWidget.h"

namespace Nit
{
    class Window;
    
    class ImGuiRenderer : public Singleton<ImGuiRenderer>
    {
    public:
        ImGuiConfigFlags ConfigFlags;
        
        ImGuiRenderer();
        ImGuiRenderer(ImGuiRenderer&& other) = delete;
        
        template<typename T, typename ...TArgs>
        std::shared_ptr<T> PushWidget(TArgs&& ...args)
        {
            return m_RootWidget->PushWidget<T>(std::forward<TArgs>(args)...);
        }
        
        void Initialize(const std::shared_ptr<Window>& window, bool bSetDefaultConfiguration = true, Delegate<void()> customConfiguration = {});
        void Update();
        void Finalize();
        
    private:
        std::weak_ptr<Window> m_Window;
        std::shared_ptr<ImGuiWidget> m_RootWidget;

        void ConfigureFlags();
        void ConfigureStyle();
    };
}