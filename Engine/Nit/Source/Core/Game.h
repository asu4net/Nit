#pragma once
#include <memory>
#include "LayerStack.h"
#include "Time.h"
#include "Utility/Singleton.h"

//TODO: Constructor of game shouldn't be public

namespace Nit
{
    class Game : public Singleton<Game>
    {
    public:
        Game();
        ~Game();
        Game(Game&&) = delete;
        
        const std::shared_ptr<LayerStack>& GetLayerStack() const { return m_LayerStack; }
        const std::shared_ptr<class Window>& GetWindow() const { return m_Window; }
        
        void Initialize();
        
    private:
        std::shared_ptr<Window> m_Window;
        class Renderer2D& m_Renderer2D;
        std::shared_ptr<LayerStack> m_LayerStack;
        Time m_Time;
    };
}