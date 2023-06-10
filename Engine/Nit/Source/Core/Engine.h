#pragma once
#include "LayerStack.h"
#include "Time.h"

namespace Nit
{
    class Engine : public Singleton<Engine>
    {
    public:
        Engine(Engine&&) = delete;

        void SetScreenWidth(const uint32_t width) { m_ScreenWidth = width; }
        uint32_t GetScreenWidth() const { return m_ScreenWidth; }

        void SetScreenHeight(const uint32_t height) { m_ScreenHeight = height; }
        uint32_t GetScreenHeight() const { return m_ScreenHeight; }
        
        void SetUseWindowScreenSize(const bool bUseWindowScreenSize) { m_bUseWindowScreenSize = bUseWindowScreenSize; }
        bool UseWindowScreenSize() const { return m_bUseWindowScreenSize; }
        
        Shared<LayerStack> GetLayerStack() const { return m_LayerStack; }
        Shared<class Window> GetWindow() const { return m_Window; }
        Shared<class World> GetWorld() const { return m_World; }
        
        void Start();
        
    private:
        Engine();
        ~Engine();

        uint32_t m_ScreenWidth = 1280;
        uint32_t m_ScreenHeight = 720;
        bool m_bUseWindowScreenSize = true;
        
        Shared<Window> m_Window;
        class Renderer2D& m_Renderer2D;
        class AudioManager& m_AudioManager;
        class AssetManager& m_AssetManager;
        Shared<LayerStack> m_LayerStack;
        Shared<World> m_World;
        Time m_Time;

        #ifdef NIT_IMGUI
        class ImGuiRenderer& m_ImGuiRenderer;
        #endif

        void TrySetScreenSize();
        
        friend class Singleton<Engine>;
    };
}
