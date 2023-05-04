#pragma once
#include "LayerStack.h"
#include "Time.h"

namespace Nit
{
    class Game : public Singleton<Game>
    {
    public:
        Game(Game&&) = delete;
        
        const Shared<LayerStack>& GetLayerStack() const { return m_LayerStack; }
        const Shared<class Window>& GetWindow() const { return m_Window; }
        
        void Initialize();
        
    private:
        Game();
        ~Game();
        
        Shared<Window> m_Window;
        class Renderer2D& m_Renderer2D;
        class AudioManager& m_AudioManager;
        class AssetManager& m_AssetManager;
        Shared<LayerStack> m_LayerStack;
        Time m_Time;

        #ifdef NIT_IMGUI
        class ImGuiRenderer& m_ImGuiRenderer;
        #endif

        friend class Singleton<Game>;
    };
}