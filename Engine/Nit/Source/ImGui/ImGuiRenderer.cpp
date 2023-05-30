#ifdef NIT_IMGUI
#include "ImGuiRenderer.h"
#include <glfw/glfw3.h>
#include "Core/Engine.h"
#include "Core/Asset/AssetManager.h"
#include "Window/Window.h"

namespace Nit
{
    ImGuiRenderer::ImGuiRenderer()
        : ConfigFlags(0)
    {
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
        
        const std::string fontLocation = CurrentDirectory() + "\\" + "Content\\Fonts\\AlbertSans-VariableFont_wght.ttf";
        io.Fonts->AddFontFromFileTTF(fontLocation.c_str(), 17);
        
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

    void ImGuiRenderer::Start(const Shared<Window>& window, bool bSetDefaultConfiguration, Delegate<void()> customConfiguration)
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
    
    void ImGuiRenderer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }
    
    void ImGuiRenderer::End()
    {
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
    
    void ImGuiRenderer::Finish()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
#endif