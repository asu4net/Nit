#include "ActorPanel.h"
#include "Scene/World.h"
#include "Scene/Components/DetailsComponent.h"
#include "Scene/Components/TransformComponent.h"

namespace Nit
{
    ActorPanel::ActorPanel() = default;

    void ActorPanel::Draw()
    {
        using namespace entt;

        ImGuiWindowClass windowClass;
        windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
        ImGui::SetNextWindowClass(&windowClass);
        
        ImGui::Begin("Actors");
        // ImGui::SetNextItemOpen(true, ImGuiCond_Once);

        const Weak<Scene> weakActiveScenePtr = World::GetActiveScenePtr();
        if (weakActiveScenePtr.expired())
            return;

        Scene& activeScene = *weakActiveScenePtr.lock();
        
        const Weak<registry> weakRegistryPtr = activeScene.GetRegistry();
        if (weakRegistryPtr.expired())
            return;
                
        const registry& registry = *weakRegistryPtr.lock();
        
        if (m_bFirstExecution && !registry.empty())
        {
            m_bFirstExecution = false;
            m_SelectedActor = {*registry.view<TransformComponent>().begin(), weakRegistryPtr};
        }
        
        registry.each([&](const entity entity)
        {
            const Actor actor = { entity, weakRegistryPtr };
            const ImGuiTreeNodeFlags flags = ((m_SelectedActor.IsValid() && m_SelectedActor == actor) ?
                ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
            
            const auto& details = actor.Get<DetailsComponent>();
            
            const bool isExpanded = ImGui::TreeNodeEx(details.Name.c_str(), flags);
            
            if (ImGui::IsItemClicked())
            {
                m_SelectedActor = actor;
            }

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Delete Actor"))
                {
                    activeScene.DestroyActor(m_SelectedActor);
                    m_SelectedActor.Reset();
                }
                ImGui::EndPopup();
            }

            if (isExpanded)
            {
                ImGui::TreePop();
            }
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectedActor.Reset();
        
        if (!m_SelectedActor.IsValid())
        {
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Create Empty Actor"))
                {
                    const Actor actor = activeScene.CreateActor("Empty Actor");
                    m_SelectedActor = actor; 
                }
                ImGui::EndPopup();
            }
        }
        
        ImGui::End();
    }
}
