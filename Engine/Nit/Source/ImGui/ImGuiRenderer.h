#pragma once
#ifdef NIT_IMGUI
#include "ImGuiWidget.h"

namespace Nit
{
    class Window;
    
    class ImGuiRenderer : public Singleton<ImGuiRenderer>
    {
    public:
        ImGuiConfigFlags ConfigFlags;
        
        ImGuiRenderer(ImGuiRenderer&& other) = delete;
        
        template<typename T, typename ...TArgs>
        Shared<T> PushWidget(TArgs&& ...args)
        {
            return m_RootWidget->PushWidget<T>(std::forward<TArgs>(args)...);
        }
        
        void Initialize(const Shared<Window>& window, bool bSetDefaultConfiguration = true, Delegate<void()> customConfiguration = {});
        void Update();
        void Finalize();
        
    private:
        ImGuiRenderer();
        
        Weak<Window> m_Window;
        Shared<ImGuiWidget> m_RootWidget;

        void ConfigureFlags();
        void ConfigureStyle();

        friend class Singleton<ImGuiRenderer>;
    };
}
#endif