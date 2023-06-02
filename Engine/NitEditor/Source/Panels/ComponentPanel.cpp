#include "ComponentPanel.h"

#include "ActorPanel.h"
#include "Editor.h"
#include "Scene/Actor.h"

namespace Nit
{
    ComponentPanel::ComponentPanel(Editor* editor)
        : m_Editor(editor)
    {
    }

    void ComponentPanel::Draw()
    {
        using namespace rttr;
        
        if (!m_Editor)
            return;

        ImGui::Begin("Components");
        
        const Actor selectedActor = m_Editor->GetActorPanel()->GetSelectedActor();
        
        if (selectedActor.IsValid())
        {
            auto componentTypes = type::get<ActorComponent>().get_derived_classes();

            DrawComponent(selectedActor,type::get<DetailsComponent>());
            DrawComponent(selectedActor,type::get<TransformComponent>());
            
            for (const type& t : componentTypes)
            {
                if (t == type::get<DetailsComponent>() || t == type::get<TransformComponent>())
                    continue;
                DrawComponent(selectedActor, t);
            }
        }

        ImGui::End();
    }

    void ComponentPanel::DrawComponent(const Actor& selectedActor, const rttr::type& t)
    {
        using namespace rttr;

        if (!Scene::ComponentMetaData.contains(t) || !Scene::ComponentMetaData[t].HasFunction(selectedActor))
            return;

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);

        const bool isExpanded = ImGui::TreeNodeEx(t.get_name().to_string().c_str());

        ImGui::SameLine();
        if (ImGui::Button("+"))
        {
            ImGui::OpenPopup("ComponentSettings");
        }

        bool removeComponent = false;

        if (ImGui::BeginPopup("ComponentSettings"))
        {
            if (ImGui::MenuItem("Remove Component"))
                removeComponent = true;
            ImGui::EndPopup();
        }

        if (isExpanded)
        {
            ImGui::Spacing();

            instance component = Scene::ComponentMetaData[t].GetByCopyFunction(selectedActor);

            if (!component.is_valid())
                return;

            //draw properties
            auto properties = t.get_properties();

            for (auto& property : properties)
            {
                variant variant = property.get_value(component);

                if (!variant.is_valid())
                    continue;

                const std::string propertyName = property.get_name().to_string();

                if (property.get_type() == type::get<Vec3>())
                {
                    Vec3 propValue = variant.get_value<Vec3>();

                    if (ImGuiHelpers::DrawVec3Property(propertyName.c_str(), propValue))
                    {
                        property.set_value(component, propValue);
                        Scene::ComponentMetaData[t].SetFunction(selectedActor, component);
                    }
                    continue;
                }

                if (property.get_type() == type::get<Vec4>())
                {
                    Vec4 propValue = variant.get_value<Vec4>();

                    if (ImGuiHelpers::DrawColorProperty(propertyName.c_str(), propValue))
                    {
                        property.set_value(component, propValue);
                        Scene::ComponentMetaData[t].SetFunction(selectedActor, component);
                    }
                    continue;
                }

                if (property.get_type() == type::get<int>())
                {
                    int propValue = variant.get_value<int>();

                    if (ImGuiHelpers::DrawIntProperty(propertyName.c_str(), propValue))
                    {
                        property.set_value(component, propValue);
                        Scene::ComponentMetaData[t].SetFunction(selectedActor, component);
                    }
                    continue;
                }

                if (property.get_type() == type::get<float>())
                {
                    float propValue = variant.get_value<float>();

                    if (ImGuiHelpers::DrawFloatProperty(propertyName.c_str(), propValue))
                    {
                        property.set_value(component, propValue);
                        Scene::ComponentMetaData[t].SetFunction(selectedActor, component);
                    }
                    continue;
                }

                if (property.get_type() == type::get<bool>())
                {
                    bool propValue = variant.get_value<bool>();

                    if (ImGuiHelpers::DrawBoolProperty(propertyName.c_str(), propValue))
                    {
                        property.set_value(component, propValue);
                        Scene::ComponentMetaData[t].SetFunction(selectedActor, component);
                    }
                    continue;
                }

                if (property.get_type() == type::get<int32_t>())
                {
                    int32_t propValue = variant.get_value<int32_t>();

                    if (ImGuiHelpers::DrawInt32Property(propertyName.c_str(), propValue))
                    {
                        property.set_value(component, propValue);
                        Scene::ComponentMetaData[t].SetFunction(selectedActor, component);
                    }
                    continue;
                }

                if (property.get_type() == type::get<std::string>())
                {
                    std::string propValue = variant.get_value<std::string>();

                    if (ImGuiHelpers::DrawTextProperty(propertyName.c_str(), propValue))
                    {
                        property.set_value(component, propValue);
                        Scene::ComponentMetaData[t].SetFunction(selectedActor, component);
                    }
                    continue;
                }
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::TreePop();
        }

        if (removeComponent)
            Scene::ComponentMetaData[t].RemoveFunction(selectedActor);
    }
}
