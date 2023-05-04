#include "EditorLayer.h"
#include "ImGui/ImGuiRenderer.h"

namespace Nit
{
    EditorLayer::EditorLayer()
    {
    }

    void EditorLayer::OnInitialize()
    {
        const Shared<ImGuiWidget> widget = ImGuiRenderer::GetInstance().PushWidget<ImGuiWidget>();
        
        widget->ClearBeginEndDelegates();
        widget->UpdateEvent().Add([]()
        {
            //Temporary code
            ImGui::Begin("Menu", 0, ImGuiWindowFlags_MenuBar);
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Import", "Ctrl+I"))
                    {
                        constexpr const char* filter = "Asset (*.)\0*.\0";
                        const std::string path = Game::GetInstance().GetWindow()->OpenFile(filter);
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
        });
    }

    void EditorLayer::OnUpdate(const TimeStep& timeStep)
    {
    }
}