#pragma once
#include <Nit.h>

namespace Nit
{
    class Editor : public EngineLayer
    {
    public:
        Editor() = default;

        void OnStart() override;
        void OnUpdate(const TimeStep& timeStep) override;

    private:
        Shared<class ViewportPanel> m_ViewportPanel;
        Shared<class ActorPanel> m_ActorPanel;
    };
}
