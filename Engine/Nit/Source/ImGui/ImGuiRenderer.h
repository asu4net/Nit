#pragma once
#ifdef NIT_IMGUI

namespace Nit
{
    class Window;
    
    class ImGuiRenderer : public Singleton<ImGuiRenderer>
    {
    public:
        ImGuiConfigFlags ConfigFlags;
        ImGuiRenderer(ImGuiRenderer&& other) = delete;
        
        void Start(const Shared<Window>& window, bool bSetDefaultConfiguration = true, Delegate<void()> customConfiguration = {});
        void Begin();
        void End();
        void Finish();
        
    private:
        ImGuiRenderer();
        
        Weak<Window> m_Window;

        void ConfigureFlags();
        void ConfigureStyle();

        friend class Singleton<ImGuiRenderer>;
    };
}
#endif