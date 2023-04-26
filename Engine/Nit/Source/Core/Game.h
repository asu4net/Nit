#pragma once
#include "LayerStack.h"
#include "Time.h"

namespace Nit
{
    class Game : public Singleton<Game>
    {
    public:
        Game(Game&&) = delete;
        
        const std::shared_ptr<LayerStack>& GetLayerStack() const { return m_LayerStack; }
        const std::shared_ptr<class Window>& GetWindow() const { return m_Window; }
        
        void Initialize();
        
    private:
        Game();
        ~Game();
        std::shared_ptr<Window> m_Window;
        class Renderer2D& m_Renderer2D;
        std::shared_ptr<LayerStack> m_LayerStack;
        Time m_Time;

        friend class Singleton<Game>;
    };
}