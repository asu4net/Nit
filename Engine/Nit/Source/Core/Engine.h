#pragma once
#include "LayerStack.h"
#include "Time.h"

namespace Nit
{
    class Engine : public Singleton<Engine>
    {
    public:
        Engine(Engine&&) = delete;
        
        Shared<LayerStack> GetLayerStack() const { return m_LayerStack; }
        Shared<class Window> GetWindow() const { return m_Window; }
        Shared<class World> GetWorld() const { return m_World; }
        
        void Start();
        
    private:
        Engine();
        ~Engine();
        
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

        friend class Singleton<Engine>;
    };
}
