#include "ImGuiRenderer.h"
#include "Core/Engine.h"
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <ImGuizmo.h>
#include "Implementation/imgui_impl_opengl3.h"
#include "Implementation/imgui_impl_glfw.h"

#define NIT_EDITOR_DOCKSPACE

namespace Nit::ImGuiRenderer
{
    static const char* OpenGLVersion = "#version 410";
    static String FontLocation = GetWorkingDirectory() + "\\" + "Assets\\Fonts\\AlbertSans-VariableFont_wght.ttf";
    static uint8_t FontSize = 17;

#ifdef NIT_EDITOR_DOCKSPACE
    static bool DockSpaceOpen = true;
    static bool Fullscreen = true;
    static bool Padding = false;
    static const char* DockspaceWindowName = "Nit Editor";
    static const char* DockspaceID = "Nit DockSpace";
#endif

    static void ApplyDefaultConfigFlags()
    {
        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    }

    static void ApplyDefaultStyle()
    {
        // Font
        const ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF(FontLocation.c_str(), FontSize);

        // Colors
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_WindowBg] = { 0.192f, 0.2f, 0.219f, 1.f };
        style.Colors[ImGuiCol_TabUnfocusedActive] = { 0.168f, 0.176f, 0.192f, 1.f };
        style.Colors[ImGuiCol_Tab] = { 0.168f, 0.176f, 0.192f, 1.f };
        style.Colors[ImGuiCol_TabHovered] = { 0.656f, 0.656f, 0.656f, 1.f };
        style.Colors[ImGuiCol_TabActive] = { 0.211f, 0.215f, 0.239f, 1.f };
        style.Colors[ImGuiCol_TitleBg] = { 0.168f, 0.176f, 0.192f, 1.f };
        style.Colors[ImGuiCol_TitleBgActive] = { 0.1f, 0.1f, 0.1f, 1.f };
        style.FrameRounding = 9.f;
        style.Colors[ImGuiCol_FrameBg] = { 0.1f, 0.1f, 0.1f, 1.f };
        style.Colors[ImGuiCol_Button] = { 0.356f, 0.356f, 0.416f, 1.f };
    }

    void Init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        
        ApplyDefaultConfigFlags();
        ApplyDefaultStyle();

        GLFWwindow* windowHandler = static_cast<GLFWwindow*>(Engine::GetWindow().GetHandler());
        ImGui_ImplGlfw_InitForOpenGL(windowHandler, true);
        ImGui_ImplOpenGL3_Init(OpenGLVersion);
    }

    void Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

#ifdef NIT_EDITOR_DOCKSPACE
        
        ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (Fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();

            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

            windowFlags |= ImGuiWindowFlags_NoTitleBar;
            windowFlags |= ImGuiWindowFlags_NoCollapse;
            windowFlags |= ImGuiWindowFlags_NoResize;
            windowFlags |= ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
            windowFlags |= ImGuiWindowFlags_NoNavFocus;
        }
        else
            dockSpaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;

        if (dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
            windowFlags |= ImGuiWindowFlags_NoBackground;

        if (!Padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        // ---------------------------------------------------------
        //  ImGui Dock Space Begin
        // ---------------------------------------------------------

        ImGui::Begin(DockspaceWindowName, &DockSpaceOpen, windowFlags);

        if (!Padding)
            ImGui::PopStyleVar();

        if (Fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const ImGuiID dockSpaceId = ImGui::GetID(DockspaceID);
            ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);
        }

#endif
    }
    
    void End()
    {
#ifdef NIT_EDITOR_DOCKSPACE

        ImGui::End();

#endif

        // ---------------------------------------------------------
        //  ImGui Dock Space End
        // ---------------------------------------------------------

        ImGuiIO& io = ImGui::GetIO();
        
        if (Engine::HasWindow())
        {
            const Vector2 windowSize = Engine::GetWindow().GetSize();
            io.DisplaySize = { windowSize.x, windowSize.y };
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

    void Finish()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}