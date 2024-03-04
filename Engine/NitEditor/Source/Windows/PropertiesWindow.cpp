#include "PropertiesWindow.h"
#include <imgui.h>
#include "ImGui/ImGuiUtils.h"
#include "Entity/Entity.h"
#include "imgui_internal.h"
#include "Component/ComponentReflection.h"
#include "EditorSystem.h"
#include "Component/TransformComponent.h"
#include "Component/SceneComponent.h"
#include "Component/IDComponent.h"
#include "Component/NameComponent.h"

namespace Nit::PropertiesWindow
{
    void ShowComponent(Entity owner, const Type& type)
    {
        const SharedPtr<ComponentType> componentType = ComponentReflection::GetComponentOfType(type);

        if (!componentType || !componentType->HasComponent(owner))
        {
            return;
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);

        const bool isExpanded = ImGui::TreeNodeEx(type.get_name().to_string().c_str());

        ImGui::SameLine();
        if (ImGui::Button("+"))
        {
            ImGui::OpenPopup("ComponentSettings");
        }

        bool removeComponent = false;

        if (ImGui::BeginPopup("ComponentSettings"))
        {
            const bool canRemove = type != Type::get<TransformComponent>();

            if (canRemove && ImGui::MenuItem("Remove Component"))
                removeComponent = true;

            ImGui::EndPopup();
        }

        if (isExpanded)
        {
            ImGui::Spacing();

            Instance component = componentType->GetComponentByCopy(owner);
            ImGui::Class(component, false);
            componentType->SetComponentData(owner, component);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::TreePop();
        }

        if (removeComponent)
            componentType->RemoveComponent(owner);
    }

    void Show(bool* bShow)
    {
        ImGuiWindowClass windowClass;
        windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
        ImGui::SetNextWindowClass(&windowClass);

        ImGui::Begin("Properties", bShow);

        if (Input::IsEnabled() && ImGui::IsWindowFocused())
            Input::SetEnabled(false);

        Entity selectedEntity = EditorSystem::GetSelectedEntity();

        if (selectedEntity.IsValid())
        {
            ShowComponent(selectedEntity, Type::get<NameComponent>());
            ShowComponent(selectedEntity, Type::get<TransformComponent>());

            ComponentReflection::Each({
            [&selectedEntity](Type type, const SharedPtr<ComponentType>& componentType)
            {
                if (type == Type::get<TransformComponent>() 
                    || type == Type::get<IDComponent>() 
                    || type == Type::get<SceneComponent>()
                    || type == Type::get<NameComponent>())
                {
                  return;
                }
                ShowComponent(selectedEntity, type);
            }});

            if (ImGui::CenteredButton("Add Component"))
            {
                ImGui::OpenPopup("AddComponent");
            }

            if (ImGui::BeginPopup("AddComponent"))
            {
                ComponentReflection::Each({
                [&selectedEntity](Type type, const SharedPtr<ComponentType>& componentType)
                {
                  if (!componentType->HasComponent(selectedEntity) && ImGui::MenuItem(type.get_name().to_string().c_str()))
                  {
                      componentType->AddDefaultComponent(selectedEntity);
                  }
                }});
                ImGui::EndPopup();
            }
        }

        ImGui::End();
    }
}