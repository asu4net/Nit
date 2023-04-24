#pragma once
#include <memory>
#include "LayerStack.h"
#include "Time.h"
#include "Utility/Singleton.h"

namespace Nit
{
    class Game : public Singleton<Game>
    {
    public:
        const std::shared_ptr<LayerStack>& GetLayerStack() const { return m_LayerStack; }
        const std::shared_ptr<class Window>& GetWindow() const { return m_Window; }
        const Time& GetTime() const { return m_Time; }
        
        template<typename T, typename ...TArgs>
        std::shared_ptr<T> PushLayer(TArgs&& ...args)
        {
            return m_LayerStack->PushLayer<T>(std::forward<TArgs>(args)...);
        }
        
        void Start();
        
    private:
        Game();
        ~Game();
        
        std::shared_ptr<LayerStack> m_LayerStack;
        std::shared_ptr<Window> m_Window;
        class Renderer2D& m_Renderer2D;
        Time m_Time;

        friend class Singleton;
    };
}
