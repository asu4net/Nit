#include "Editor.h"
#include "ImGui/ImGuiRenderer.h"
#include "Scene/Components/EditorCameraComponent.h"

namespace Nit
{
    void Editor::OnStart()
    {
        World::GetActiveScenePtr().lock()->SetRuntimeEnabled(false);
        
        const Actor editorCamera = World::GetActiveScenePtr().lock()->CreateActor("EditorCamera");
        editorCamera.Add<EditorCameraComponent>();
        editorCamera.Get<TransformComponent>().Position = VecBack * 3.f;
        
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
        });

        //World::GetActiveScene().SetRuntimeEnabled(true);
    }

    void Editor::OnUpdate(const TimeStep& timeStep)
    {
    }
}
