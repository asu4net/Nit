#include "HierarchyWindow.h"
#include <imgui.h>
#include "ViewportWindow.h"
#include "Entity/World.h"
#include "Entity/Entity.h"
#include "Component/NameComponent.h"
#include "EditorSystem.h"
#include "EditorCameraSystem.h"
#include "Component/TransformComponent.h"

namespace Nit::HierarchyWindow
{
    void ShowEntityNode(Entity entity, bool bParentExpanded)
    {
        if (!bParentExpanded) return;

        Entity selectedEntity = EditorSystem::GetSelectedEntity();

        const auto& name = entity.GetName();
        const ImGuiTreeNodeFlags flags = ((selectedEntity.IsValid() && selectedEntity == entity) ?
            ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

        const bool bIsExpanded = ImGui::TreeNodeEx(name.GetRawString(), flags);

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Destroy Entity"))
            {
                World::DestroyEntity(entity);
            }
            if (ImGui::MenuItem("Clone Entity"))
            {
                World::CloneEntity(entity);
            }
            ImGui::EndPopup();
        }

        if (ImGui::IsItemClicked())
        {
            EditorSystem::SetSelectedEntity(entity);
        }

        if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            EditorCameraSystem::SetPosition(entity.GetTransform().Position);
        }

        if (bIsExpanded)
        {
            ImGui::TreePop();
        }
    }

    void ShowSceneNode(const TString& sceneName, Scene* scene)
    {
        const bool bIsExpanded = ImGui::TreeNodeEx(sceneName.c_str(), ImGuiTreeNodeFlags_DefaultOpen);

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Create Empty Entity"))
            {
                EntityCreationParams params;
                params.SceneOwner = sceneName;
                World::CreateEntity(params);
            }
            ImGui::EndPopup();
        }

        EntityArray entities = scene->GetEntities();

        for (Entity& entity : entities)
        {
            ShowEntityNode(entity, bIsExpanded);
        }

        if (bIsExpanded)
        {
            ImGui::TreePop();
        }
    }

    void ShowGlobalEntities()
    {
        const bool bIsExpanded = ImGui::TreeNodeEx("Global", ImGuiTreeNodeFlags_DefaultOpen);
        auto& globalEntities = World::GetGlobalEntities();

        for (Entity entity : globalEntities)
        {
            ShowEntityNode(entity, bIsExpanded);
        }

        if (bIsExpanded)
        {
            ImGui::TreePop();
        }
    }

    void ShowSceneEntities()
    {
        auto& openedScenes = World::GetOpenedScenes();

        for (auto&[sceneName, scene] : openedScenes)
        {
            ShowSceneNode(sceneName, scene);
        }
    }

    void Show(bool* bShow)
    {
        if (Input::IsEnabled() && (ImGui::IsWindowFocused() || ImGui::IsWindowHovered()))
            Input::SetEnabled(false);

        ImGui::Begin("Hierarchy", bShow);
        
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
        {
            EditorSystem::SetSelectedEntity({});
        }

        if (!World::HasRegistry())
        {
            return;
        }

        ShowGlobalEntities();
        ShowSceneEntities();

        ImGui::End();
    }
}