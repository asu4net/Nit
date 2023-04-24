#include "Game.h"
#include "Rendering/Renderer2D.h"
#include "Window/Window.h"

namespace Nit
{
    Game::Game()
        : m_LayerStack(std::make_shared<LayerStack>())
        , m_Window(Window::CreateAndInitialize())
        , m_Renderer2D(Renderer2D::CreateAndInitialize(m_Window))
    {
    }

    Game::~Game()
    {
        Renderer2D::FinalizeAndDestroy();
        m_Window->Finalize();
    }

    void Game::Start()
    {
        while (m_Window->IsOpened())
        {
            m_Renderer2D.ClearScreen(m_Window->GetBackgroundColor());
            m_LayerStack->UpdateLayers(m_Time.CalculateTimeStep());
            m_Window->Update();
        }
    }
}
