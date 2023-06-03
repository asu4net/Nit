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

        ImGuiWindowClass windowClass;
        windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
        ImGui::SetNextWindowClass(&windowClass);

        if (!m_bOpened)
            return;
        
        ImGui::Begin("Components");

        const Actor selectedActor = m_Editor->GetActorPanel()->GetSelectedActor();

        if (selectedActor.IsValid())
        {
            auto componentTypes = type::get<ActorComponent>().get_derived_classes();

            DrawComponent(selectedActor, type::get<DetailsComponent>());
            DrawComponent(selectedActor, type::get<TransformComponent>());

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
            const bool canRemove = t != type::get<TransformComponent>() && t != type::get<DetailsComponent>();

            if (canRemove && ImGui::MenuItem("Remove Component"))
                removeComponent = true;
            
            ImGui::EndPopup();
        }

        if (isExpanded)
        {
            ImGui::Spacing();

            instance component = Scene::ComponentMetaData[t].GetByCopyFunction(selectedActor);
            DrawClass(component, t, false);
            Scene::ComponentMetaData[t].SetFunction(selectedActor, component);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::TreePop();
        }

        if (removeComponent)
            Scene::ComponentMetaData[t].RemoveFunction(selectedActor);
    }

    void ComponentPanel::DrawClass(const rttr::instance& instance, const rttr::type& type, bool drawAsTree)
    {
        if (!instance.is_valid())
            return;

        if (!drawAsTree)
        {
            DrawProperties(instance, type);
            return;
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);

        if (ImGui::TreeNodeEx(type.get_name().to_string().c_str()))
        {
            ImGuiHelpers::DrawSpacing(3);
            DrawProperties(instance, type);
            ImGuiHelpers::DrawSpacing(4);
            ImGui::TreePop();
        }
    }

    void ComponentPanel::DrawProperties(const rttr::instance& instance, const rttr::type& type)
    {
        //draw properties
        auto properties = type.get_properties();

        for (auto& property : properties)
        {
            rttr::variant variant = property.get_value(instance);

            if (!variant.is_valid())
                continue;

            const std::string propertyName = property.get_name().to_string();
            const rttr::type propertyType = property.get_type();

            if (propertyType == rttr::type::get<Vec2>())
            {
                Vec2 propValue = variant.get_value<Vec2>();

                if (ImGuiHelpers::DrawVec2Property(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == rttr::type::get<Vec3>())
            {
                Vec3 propValue = variant.get_value<Vec3>();

                if (ImGuiHelpers::DrawVec3Property(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == rttr::type::get<Vec4>())
            {
                Vec4 propValue = variant.get_value<Vec4>();

                if (ImGuiHelpers::DrawColorProperty(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == rttr::type::get<int>())
            {
                int propValue = variant.get_value<int>();

                if (ImGuiHelpers::DrawIntProperty(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == rttr::type::get<float>())
            {
                float propValue = variant.get_value<float>();

                if (ImGuiHelpers::DrawFloatProperty(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == rttr::type::get<bool>())
            {
                bool propValue = variant.get_value<bool>();

                if (ImGuiHelpers::DrawBoolProperty(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == rttr::type::get<int32_t>())
            {
                int32_t propValue = variant.get_value<int32_t>();

                if (ImGuiHelpers::DrawInt32Property(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == rttr::type::get<uint32_t>())
            {
                uint32_t propValue = variant.get_value<uint32_t>();

                if (ImGuiHelpers::DrawUInt32Property(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == rttr::type::get<std::string>())
            {
                std::string propValue = variant.get_value<std::string>();

                if (ImGuiHelpers::DrawTextProperty(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType.is_valid() && propertyType.is_enumeration())
            {
                rttr::enumeration propEnum = propertyType.get_enumeration();
                if (!propEnum.is_valid())
                    continue;

                auto enumNames = propEnum.get_names();
                std::vector<std::string> enumNamesStr;

                for (const auto& enumName : enumNames)
                {
                    enumNamesStr.push_back(enumName.to_string());
                }

                std::string selectedEnum = variant.to_string();
                ImGuiHelpers::DrawEnumProperty(propertyName.c_str(), selectedEnum, enumNamesStr);
                rttr::variant enumValue = propEnum.name_to_value(selectedEnum);
                property.set_value(instance, enumValue);
                continue;
            }

            if (propertyType == rttr::type::get<Quat>())
            {
                Vec3 eulerAngles = Math::Degrees(Math::EulerAngles(variant.get_value<Quat>()));
                if (ImGuiHelpers::DrawVec3Property(propertyName.c_str(), eulerAngles))
                {
                    Quat rotation = Quat(Math::Radians(eulerAngles));
                    property.set_value(instance, rotation);
                }
                continue;
            }

            if (propertyType == rttr::type::get<Id>())
            {
                //TODO
                continue;
            }

            if (propertyType == rttr::type::get<AssetLink>())
            {
                //TODO
                continue;
            }

            if (propertyType.is_class())
            {
                DrawClass(variant, propertyType);
                property.set_value(instance, variant);
            }
        }
    }
}
