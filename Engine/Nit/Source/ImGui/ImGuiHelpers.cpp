#ifdef NIT_IMGUI
#include "ImGuiHelpers.h"

namespace Nit::ImGuiHelpers
{
    bool Vec3Handler(const char* name, Vec3& vector, const float resetValue, const float speed, const float columnWidth)
    {
        ImGui::PushID(name);
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(name);
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 6.f});
        
        const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        const ImVec2 buttonSize = {lineHeight + 3.f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, {1, 0.3f, 0.3f, 1});

        if (ImGui::Button("X", buttonSize))
            vector.x = resetValue;

        ImGui::PopStyleColor();

        ImGui::SameLine();
        const bool bX = ImGui::DragFloat("##X", &vector.x, speed, 0, 0, "%.1f");
        ImGui::PopItemWidth();

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, {0.368f, 0.737f, 0.521f, 1});

        if (ImGui::Button("Y", buttonSize))
            vector.y = resetValue;

        ImGui::PopStyleColor();

        ImGui::SameLine();
        const bool bY = ImGui::DragFloat("##Y", &vector.y, speed, 0, 0, "%.1f");
        ImGui::PopItemWidth();

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, {0.3f, 0.3f, 1, 1});

        if (ImGui::Button("Z", buttonSize))
            vector.z = resetValue;

        ImGui::PopStyleColor();

        ImGui::SameLine();
        const bool bZ = ImGui::DragFloat("##Z", &vector.z, speed, 0, 0, "%.1f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);

        ImGui::PopID();

        return bX || bY || bZ;
    }
}
#endif