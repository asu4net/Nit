#include "ImGuiRenderer.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include <glfw/glfw3.h>
#include "Implementation/imgui_impl_glfw.h"
#include "Implementation/imgui_impl_opengl3.h"
#include "Window/Window.h"

namespace Nit
{
    ImGuiRenderer::ImGuiRenderer()
        : ConfigFlags(0)
    {
    }

    ImGuiRenderer& ImGuiRenderer::CreateAndInitialize(const std::shared_ptr<Window>& window, bool bSetDefaultConfiguration,
        const Delegate<void()>& customConfiguration)
    {
        ImGuiRenderer& imGuiRenderer = Create();
        imGuiRenderer.Initialize(window, bSetDefaultConfiguration, customConfiguration);
        return imGuiRenderer;
    }

    void ImGuiRenderer::FinalizeAndDestroy()
    {
        ImGuiRenderer& imGuiRenderer = GetInstance();
        imGuiRenderer.Finalize();
        Destroy();
    }

    void ImGuiRenderer::DestroyRootWidget()
    {
        if (!m_RootWidget) return;
        m_RootWidget->Destroy();
        m_RootWidget.reset();
        m_RootWidget = nullptr;
    }

    void ImGuiRenderer::ConfigureFlags()
    {
        // Settings configuration
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ConfigFlags;
    }

    void ImGuiRenderer::ConfigureStyle()
    {
        // Font
        const ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF(R"(Content\Fonts\AlbertSans-VariableFont_wght.ttf)", 17);
        
        // Colors
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_WindowBg] = {0.192f, 0.2f, 0.219f, 1.f};
        style.Colors[ImGuiCol_TabUnfocusedActive] = {0.168f, 0.176f, 0.192f, 1.f};
        style.Colors[ImGuiCol_Tab] = {0.168f, 0.176f, 0.192f, 1.f};
        style.Colors[ImGuiCol_TabHovered] = {0.656f, 0.656f, 0.656f, 1.f};
        style.Colors[ImGuiCol_TabActive] = {0.211f, 0.215f, 0.239f, 1.f};
        style.Colors[ImGuiCol_TitleBgActive] = {0.168f, 0.176f, 0.192f, 1.f};
        style.Colors[ImGuiCol_TitleBg] = {0.1f, 0.1f, 0.1f, 1.f};
        style.FrameRounding = 9.f;
        style.Colors[ImGuiCol_FrameBg] = {0.1f, 0.1f, 0.1f, 1.f};
        style.Colors[ImGuiCol_Button] = {0.356f, 0.356f, 0.416f, 1.f};
    }

    void ImGuiRenderer::Initialize(const std::shared_ptr<Window>& window, bool bSetDefaultConfiguration, Delegate<void()> customConfiguration)
    {
        m_Window = window;
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        if (bSetDefaultConfiguration)
        {
            ConfigureFlags();
            ConfigureStyle();
        }
        customConfiguration();
        GLFWwindow* windowHandler = static_cast<GLFWwindow*>(window->GetHandler());
        ImGui_ImplGlfw_InitForOpenGL(windowHandler, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiRenderer::DrawWidgets()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
        
        // static bool dockSpaceOpen = true;
        // static bool fullscreen = true;
        // static bool padding = false;
        // static ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;
        //
        // ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        // if (fullscreen)
        // {
        //     const ImGuiViewport* viewport = ImGui::GetMainViewport();
        //     ImGui::SetNextWindowPos(viewport->WorkPos);
        //     ImGui::SetNextWindowSize(viewport->WorkSize);
        //     ImGui::SetNextWindowViewport(viewport->ID);
        //     ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        //     ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        //     windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        //         ImGuiWindowFlags_NoMove;
        //     windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        // }
        // else
        // {
        //     dockSpaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        // }
        //
        // if (dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        //     windowFlags |= ImGuiWindowFlags_NoBackground;
        //
        // if (!padding)
        //     ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        //
        // for (const auto& widget : m_widgets)
        // {
        //     if (!widget->enabled) continue;
        //     ImGui::Begin(widget->GetName(), widget->opened, widget->flags);
        //     widget->OnUpdate();
        //     ImGui::End();
        // }

        //---------------------------------------------------------
        // ImGui Dock Space Begin
        //---------------------------------------------------------
        
        // ImGui::Begin("Nit Graphics", &dockSpaceOpen, windowFlags);
        //
        // if (!padding)
        //     ImGui::PopStyleVar();
        //
        // if (fullscreen)
        //     ImGui::PopStyleVar(2);
        //
        // // Submit the DockSpace
        // ImGuiIO& io = ImGui::GetIO();
        // if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        // {
        //     const ImGuiID dockSpaceId = ImGui::GetID("NitGraphicsDockSpace");
        //     ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);
        // }
        // else
        // {
        //     printf("Docking disabled");
        // }
        //
        m_RootWidget->Update();
        //
        // ImGui::End();

        //---------------------------------------------------------
        // ImGui Dock Space End
        //---------------------------------------------------------

        ImGuiIO& io = ImGui::GetIO();
        if (!m_Window.expired())
        {
            const auto window = m_Window.lock();
            io.DisplaySize = {static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight())};
        }

        ImGui::Render();
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupCurrentContext);
        }
    }

    void ImGuiRenderer::Finalize()
    {
        m_RootWidget->Destroy();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}