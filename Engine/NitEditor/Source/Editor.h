#pragma once
#include <Nit.h>

namespace Nit
{
    class ViewportPanel;

    class Editor : public EngineLayer
    {
    public:
        Editor() = default;

        void OnStart() override;
        void OnUpdate(const TimeStep& timeStep) override;

    private:
        Shared<ViewportPanel> m_ViewportPanel;
    };
}
