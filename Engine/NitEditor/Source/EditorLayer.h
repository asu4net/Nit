#pragma once
#include <Nit.h>

namespace Nit
{
    class EditorLayer : public GameLayer
    {
    public:
        class ImGuiRenderer& ImGuiRenderer;
        
        EditorLayer();

        void OnInitialize() override;
        void OnUpdate(const TimeStep& timeStep) override;
    };
}