#include "SceneWindow.h"
#include <imgui.h>
#include "Entity\World.h"
#include "ImGui\ImGuiUtils.h"

namespace Nit::SceneWindow
{
    void Show(bool* bShow)
    {
        if (Input::IsEnabled() && (ImGui::IsWindowFocused() || ImGui::IsWindowHovered()))
            Input::SetEnabled(false);

        ImGui::Begin("Scene", bShow, ImGuiWindowFlags_NoCollapse);

        static const auto& allScenes = World::GetAllScenes();
        DynamicArray<String> allSceneNames;

        for (const auto& [name, scene] : allScenes)
            allSceneNames.push_back(name);
        
        static String selectedScene = !World::GetOpenedScenes().empty() ?
            World::GetOpenedScenes().begin()->second->GetAssetData().Name : "";

        ImGui::Combo("All Scenes", selectedScene, allSceneNames);

        if (!selectedScene.empty())
        {
            ImGui::Spacing(2);
            if (ImGui::Button("Open") && !World::IsSceneOpened(selectedScene)) World::OpenScene(selectedScene);
            ImGui::SameLine();
            if (ImGui::Button("Close") && World::IsSceneOpened(selectedScene)) World::CloseScene(selectedScene);
            ImGui::SameLine();
            if (ImGui::Button("Save")) World::SaveScene(selectedScene);
        }

        ImGui::Spacing(2); ImGui::Separator(); ImGui::Spacing(2);

        static String sceneToCreate; ImGui::InputText("New Scene", sceneToCreate); ImGui::SameLine();
        ImGui::Spacing(2);
        if (ImGui::Button("Create") && !sceneToCreate.empty() && !World::IsSceneCreated(sceneToCreate)) World::CreateScene(sceneToCreate);

        ImGui::End();
    }
}