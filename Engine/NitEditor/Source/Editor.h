#pragma once
#include <Nit.h>

namespace Nit
{
    class Editor : public EngineLayer
    {
    public:
        Editor() = default;

        Shared<class ActorPanel> GetActorPanel() const { return m_ActorPanel; }
        
        void OnStart() override;
        void OnUpdate(const TimeStep& timeStep) override;

    private:
        Shared<class ViewportPanel> m_ViewportPanel;
        Shared<class ActorPanel> m_ActorPanel;
        Shared<class ComponentPanel> m_ComponentPanel;
    };
}
