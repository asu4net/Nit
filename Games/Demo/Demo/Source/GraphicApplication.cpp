#include "GraphicApplication.h"
#include "Camera/ViewportCameraController.h"

namespace Nit
{
    GraphicApplication::GraphicApplication()
        : m_Window(Window::CreateAndInitialize())
        , Renderer2D(Renderer2D::CreateAndInitialize(m_Window))
        , ImGuiRenderer(ImGuiRenderer::CreateAndInitialize(m_Window))
        , m_LogicCamera(std::make_shared<LogicCamera>(m_Window))
        , m_LastFrameTime(Time::GetTime())
    {
    }
    
    GraphicApplication::~GraphicApplication()
    {
        Renderer2D::FinalizeAndDestroy();
        ImGuiRenderer::FinalizeAndDestroy();
        m_Window->Finalize();
    }

    void GraphicApplication::Create()
    {
        m_RootWidget = ImGuiRenderer::GetInstance().CreateRootWidget<ImGuiWidget>("Settings:");
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

    void GraphicApplication::Update(const float deltaTime)
    {
        Renderer2D.ClearScreen();
        m_LogicCamera->Update(deltaTime);
        OnUpdate(deltaTime);
        ImGuiRenderer.DrawWidgets();
        m_Window->Update();
    }
}