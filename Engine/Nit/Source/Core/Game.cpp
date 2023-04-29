#include "Game.h"
#include "Asset.h"
#include "YAMLSerializer.h"
#include "Rendering/Renderer2D.h"
#include "Window/Window.h"

namespace Nit
{
    Game::Game()
        : m_Window(Window::Create())
        , m_Renderer2D(Renderer2D::CreateSingleton())
        , m_LayerStack(std::make_shared<LayerStack>())
    {}

    void Game::Initialize()
    {
        Asset asset{"alex", "jaja", Id()};
        std::string result;
        YAMLSerializer::SerializeObject(asset, "Mi asset", result);
        std::cout << result << std::endl;
        
        m_Window->Initialize();
        
        m_Renderer2D.Initialize(m_Window);
        m_LayerStack->Initialize();
        
       while (m_Window->IsOpened())
        {
            m_Renderer2D.ClearScreen(m_Window->GetBackgroundColor());
            m_LayerStack->Update(m_Time.CalculateTimeStep());
            m_Window->Update();
        }

        m_LayerStack->Finalize();
        m_Renderer2D.Finalize();
        m_Window->Finalize();
    }

    Game::~Game()
    {
        Renderer2D::Destroy();
    }
}