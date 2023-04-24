#include "GraphicApplication.h"
#include "Camera/ViewportCameraController.h"

namespace Nit
{
    Game::Game()
        : m_Window(Window::CreateAndInitialize())
        , Renderer2D(Renderer2D::CreateAndInitialize(m_Window))
        , m_LogicCamera(std::make_shared<LogicCamera>(m_Window))
        , m_LastFrameTime(Time::GetTime())
    {
    }
    
    Game::~Game()
    {
        Renderer2D::FinalizeAndDestroy();
        m_Window->Finalize();
    }

    void Game::Create()
    {
        m_LogicCamera->AddController<ViewportCameraController>();
        OnCreate();
        
        while (m_Window->IsOpened())
        {
            const double time = Time::GetTime();
            const float deltaTime = static_cast<float>(time - m_LastFrameTime);
            m_LastFrameTime = time;
            Update(deltaTime);
        }
    }

    void Game::Update(const float deltaTime)
    {
        Renderer2D.ClearScreen();
        m_LogicCamera->Update(deltaTime);
        OnUpdate(deltaTime);
        m_Window->Update();
    }
}