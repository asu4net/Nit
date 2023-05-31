#include "Editor.h"

#include "Panels/ViewportPanel.h"
#include "Scene/Components/EditorCameraComponent.h"

namespace Nit
{
    void Editor::OnStart()
    {
        if (!World::HasActiveScene()) return;
        
        World::GetActiveScene().SetRuntimeEnabled(false);
        
        const Actor editorCamera = World::GetActiveScenePtr().lock()->CreateActor("EditorCamera");
        editorCamera.Add<EditorCameraComponent>();
        editorCamera.Get<TransformComponent>().Position = VecBack * 3.f;

        m_ViewportPanel = CreateShared<ViewportPanel>();
    }
    
    void Editor::OnUpdate(const TimeStep& timeStep)
    
    {
        static bool dockSpaceOpen = true;
        static bool fullscreen = true;
        static bool padding = false;
        static ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;
        
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockSpaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }
        
        if (dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
            windowFlags |= ImGuiWindowFlags_NoBackground;
        
        if (!padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        // ---------------------------------------------------------
        //  ImGui Dock Space Begin
        // ---------------------------------------------------------
        
        ImGui::Begin("Nit Graphics", &dockSpaceOpen, windowFlags);
        
        if (!padding)
            ImGui::PopStyleVar();
        
        if (fullscreen)
            ImGui::PopStyleVar(2);
        
        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const ImGuiID dockSpaceId = ImGui::GetID("NitGraphicsDockSpace");
            ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);
        }
        else
        {
            printf("Docking disabled");
        }
        
        m_ViewportPanel->Draw();
        
        //Temporary code
        ImGui::Begin("Menu", 0, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Import", "Ctrl+I"))
                {
                    constexpr const char* filter = "Asset (*.)\0*.\0";
                    const std::string path = Engine::GetInstance().GetWindow()->OpenFile(filter);
                    if (!path.empty())
                    {
                        AssetManager::GetInstance().ImportAsset(path);
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        ImGui::End();
        
        // ---------------------------------------------------------
        //  ImGui Dock Space End
        // ---------------------------------------------------------
        ImGui::End();
    }
}
