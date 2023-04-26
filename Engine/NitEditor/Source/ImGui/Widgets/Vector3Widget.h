#pragma once
#include "ImGui/ImGuiWidget.h"

namespace Nit
{
    class Vector3Widget : public ImGuiWidget
    {
    public:
        float ResetValue;
        float Speed;

        Vector3Widget(glm::vec3& vector, const std::string& name = "ImGuiWidget", float columnWidth = 80.f, bool* opened = nullptr, ImGuiWindowFlags flags = 0);

    protected:
        void OnCreate() override;
        void OnUpdate() override;

    private:
        glm::vec3& m_Vector;
        float m_ColumnWidth;
    };
}
