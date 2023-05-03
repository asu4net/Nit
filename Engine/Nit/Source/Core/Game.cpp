#include "Game.h"

#include "Asset/AssetManager.h"
#include "Audio/AudioManager.h"
#include "Rendering/Renderer2D.h"
#include "Window/Window.h"

namespace Nit
{
    Game::Game()
        : m_Window(Window::Create())
        , m_Renderer2D(Renderer2D::CreateSingleton())
        , m_AudioManager(AudioManager::CreateSingleton())
        , m_AssetManager(AssetManager::CreateSingleton())
        , m_LayerStack(std::make_shared<LayerStack>())
    {}

    void Game::Initialize()
    {
        m_Window->Initialize();
        m_AudioManager.Initialize();
        m_AssetManager.Initialize();
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
        m_AssetManager.Finalize();
        m_AudioManager.Finalize();
        m_Window->Finalize();
    }

    Game::~Game()
    {
        AssetManager::Destroy();
        Renderer2D::Destroy();
    }
}