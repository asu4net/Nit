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
            
            for (const type& t : componentTypes)
            {
                if (!Scene::ComponentMetaData.contains(t) || !Scene::ComponentMetaData[t].HasFunction(selectedActor))
                    continue;

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
                        continue;
                    
                    //draw properties
                    auto properties = t.get_properties();
                    
                    for (auto& property : properties)
                    {
                        if (property.get_type() == type::get<Vec3>())
                        {
                            variant variant = property.get_value(component);

                            if (!variant.is_valid())
                                continue;

                            Vec3 vecPtr = variant.get_value<Vec3>();
                            if (ImGuiHelpers::Vec3Handler(property.get_name().to_string().c_str(), vecPtr))
                            {
                                property.set_value(component, vecPtr);
                                Scene::ComponentMetaData[t].SetFunction(selectedActor, component);
                            }
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

        ImGui::End();
    }
}
