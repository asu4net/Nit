#pragma once
#include "ImGui/ImGuiWidget.h"

namespace Nit
{
    class DockSpaceWidget : public ImGuiWidget
    {
    public:
        void OnUpdate() override;
    };
}