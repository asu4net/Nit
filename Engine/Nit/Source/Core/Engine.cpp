#include "Engine.h"
#include "Asset/AssetManager.h"
#include "Audio/AudioManager.h"
#include "ImGui/ImGuiRenderer.h"
#include "Rendering/Renderer2D.h"
#include "Scene/World.h"
#include "Window/Window.h"

namespace Nit
{
    Engine::Engine()
        : m_Window(Window::Create())
        , m_Renderer2D(Renderer2D::CreateSingleton())
        , m_AudioManager(AudioManager::CreateSingleton())
        , m_AssetManager(AssetManager::CreateSingleton())
        , m_LayerStack(std::make_shared<LayerStack>())
        #ifdef NIT_IMGUI
        , m_ImGuiRenderer(ImGuiRenderer::CreateSingleton())
        #endif
    {
        m_World = m_LayerStack->Push<World>();
    }

    void Engine::Start()
    {
        m_Window->Start();
        m_AudioManager.Start();
        m_AssetManager.Start();
        m_Renderer2D.Start(m_Window);
        #ifdef NIT_IMGUI
        m_ImGuiRenderer.Start(m_Window);
        #endif
        m_LayerStack->Start();

        TrySetScreenSize();
        
        while (m_Window->IsOpened())
        {
            #ifdef NIT_IMGUI
            m_ImGuiRenderer.Begin();
            #endif
            m_LayerStack->Update(m_Time.CalculateTimeStep());
            #ifdef NIT_IMGUI
            m_ImGuiRenderer.End();
            #endif
            m_Window->Update();
            TrySetScreenSize();
        }

        m_LayerStack->Finish();
        #ifdef NIT_IMGUI
        m_ImGuiRenderer.Finish();
        #endif
        m_Renderer2D.Finish();
        m_AssetManager.Finish();
        m_AudioManager.Finish();
        m_Window->Finish();
    }

    Engine::~Engine()
    {
        AssetManager::DestroySingleton();
        #ifdef NIT_IMGUI
        ImGuiRenderer::DestroySingleton();
        #endif
        Renderer2D::DestroySingleton();
    }

    void Engine::TrySetScreenSize()
    {
        if (m_bUseWindowScreenSize)
        {
            m_ScreenWidth = m_Window->GetWidth();
            m_ScreenHeight = m_Window->GetHeight();
        }
    }
}
