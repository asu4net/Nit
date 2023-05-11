#include "Vector3Widget.h"

#ifdef NIT_IMGUI
namespace Nit
{
    Vector3Widget::Vector3Widget(vec3& vector, const std::string& name, const float columnWidth, bool* opened, ImGuiWindowFlags flags)
        : ImGuiWidget(name, opened, flags)
        , ResetValue(0.f)
        , Speed(0.05f)
        , m_Vector(vector)
        , m_ColumnWidth(columnWidth)
    {
    }

    void Vector3Widget::OnCreate()
    {
        ClearBeginEndDelegates();
    }
    
    void Vector3Widget::OnUpdate()
    {
        ImGui::PushID(m_Name.c_str());
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, m_ColumnWidth);
        ImGui::Text(m_Name.c_str());
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 6.f});
        
        const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        const ImVec2 buttonSize = {lineHeight + 3.f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, {1, 0.3f, 0.3f, 1});

        if (ImGui::Button("X", buttonSize))
            m_Vector.x = ResetValue;

        ImGui::PopStyleColor();

        ImGui::SameLine();
        ImGui::DragFloat("##X", &m_Vector.x, Speed, 0, 0, "%.1f");
        ImGui::PopItemWidth();

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, {0.368f, 0.737f, 0.521f, 1});

        if (ImGui::Button("Y", buttonSize))
            m_Vector.y = ResetValue;

        ImGui::PopStyleColor();

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &m_Vector.y, Speed, 0, 0, "%.1f");
        ImGui::PopItemWidth();

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, {0.3f, 0.3f, 1, 1});

        if (ImGui::Button("Z", buttonSize))
            m_Vector.z = ResetValue;

        ImGui::PopStyleColor();

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &m_Vector.z, Speed, 0, 0, "%.1f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);

        ImGui::PopID();
    }
}
#endif