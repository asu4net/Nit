#include "HierarchyWindow.h"
#include <imgui.h>
#include "ViewportWindow.h"
#include "Entity/World.h"
#include "Entity/Entity.h"
#include "Component/NameComponent.h"
#include "EditorSystem.h"

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
            ImGui::EndPopup();
        }

        if (ImGui::IsItemClicked())
        {
            EditorSystem::SetSelectedEntity(entity);
        }

        if (bIsExpanded)
        {
            ImGui::TreePop();
        }
    }

    void ShowSceneNode(const String& sceneName, Scene* scene)
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

        for (Entity entity : scene->GetEntities())
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
        ImGui::Begin("Hierarchy", bShow);
        
        if (!World::HasRegistry())
        {
            return;
        }

        ShowGlobalEntities();
        ShowSceneEntities();

        ImGui::End();
    }
}