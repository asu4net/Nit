#pragma once
#include <Nit.h>
#include "Camera/LogicCamera.h"

namespace Nit
{
    class Window;
    class Camera;
    class ImGuiWidget;
    
    class Game
    {
        std::shared_ptr<Window> m_Window;   

    public:
        Renderer2D& Renderer2D;
        
        Game();
        virtual ~Game();

        const std::shared_ptr<LogicCamera>& GetCamera() const { return m_LogicCamera; }
        const std::shared_ptr<Window>& GetWindow() const { return m_Window; }
        const std::shared_ptr<ImGuiWidget>& GetRootWidget() const { return m_RootWidget; }

        void Create();
        
    protected:
        virtual void OnCreate() {}
        virtual void OnUpdate(float deltaTime) {}
        
    private:
        std::shared_ptr<LogicCamera> m_LogicCamera;
        std::shared_ptr<ImGuiWidget> m_RootWidget;
        double m_LastFrameTime;
        
        void Update(float deltaTime);
    };
}
