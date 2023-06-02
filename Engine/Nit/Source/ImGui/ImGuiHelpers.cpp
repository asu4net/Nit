#ifdef NIT_IMGUI
#include "ImGuiHelpers.h"

namespace Nit::ImGuiHelpers
{
    PropertyConfiguration g_Config =
    {
        /* ColumnWidth */ 100.f,
        /* ItemSpacing */ {0, 6.f},
        /* ColorX */ {1, 0.3f, 0.3f, 1},
        /* ColorY */ {0.368f, 0.737f, 0.521f, 1},
        /* ColorZ */ {0.3f, 0.3f, 1, 1}
    };

    bool DrawProperty(const char* name, int items, Delegate<bool()>& drawFieldsDelegate)
    {
        ImGui::PushID(name);
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, g_Config.ColumnWidth);
        ImGui::Text(name);
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths(items, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{
            g_Config.ItemSpacing.x, g_Config.ItemSpacing.y});
        
        const bool modified = drawFieldsDelegate();

        ImGui::PopStyleVar();
        ImGui::Columns(1);

        ImGui::PopID();
        return modified;
    }

    bool DrawVec3Property(const char* name, Vec3& vector, float resetValue, float speed)
    {
        Delegate<bool()> drawProperty([&]() -> bool
        {
            const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
            const ImVec2 buttonSize = {lineHeight + 3.f, lineHeight};
            
            ImGui::PushStyleColor(ImGuiCol_Button,
                {g_Config.ColorX.x, g_Config.ColorX.y, g_Config.ColorX.z, g_Config.ColorX.w});

            const bool xReset = ImGui::Button("X", buttonSize);
            if (xReset)
                vector.x = resetValue;

            ImGui::PopStyleColor();

            ImGui::SameLine();
            const bool xPressed = ImGui::DragFloat("##X", &vector.x, speed, 0, 0, "%.1f");
            ImGui::PopItemWidth();

            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button,
                {g_Config.ColorY.x, g_Config.ColorY.y, g_Config.ColorY.z, g_Config.ColorY.w});

            const bool yReset = ImGui::Button("Y", buttonSize);
            if (yReset)
                vector.y = resetValue;

            ImGui::PopStyleColor();

            ImGui::SameLine();
            const bool yPressed = ImGui::DragFloat("##Y", &vector.y, speed, 0, 0, "%.1f");
            ImGui::PopItemWidth();

            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button,
                {g_Config.ColorZ.x, g_Config.ColorZ.y, g_Config.ColorZ.z, g_Config.ColorZ.w});

            const bool zReset = ImGui::Button("Z", buttonSize);
            if (zReset)
                vector.z = resetValue;

            ImGui::PopStyleColor();

            ImGui::SameLine();
            const bool zPressed = ImGui::DragFloat("##Z", &vector.z, speed, 0, 0, "%.1f");
            ImGui::PopItemWidth();

            return xPressed || yPressed || zPressed || xReset || yReset || zReset;
        });
        
        return DrawProperty(name, 3, drawProperty);
    }

    bool DrawVec2Property(const char* name, Vec3& vector, float resetValue, float speed)
    {
        Delegate<bool()> drawProperty([&]() -> bool
        {
            const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
            const ImVec2 buttonSize = {lineHeight + 3.f, lineHeight};
            
            ImGui::PushStyleColor(ImGuiCol_Button,
                {g_Config.ColorX.x, g_Config.ColorX.y, g_Config.ColorX.z, g_Config.ColorX.w});

            const bool xReset = ImGui::Button("X", buttonSize);
            if (xReset)
                vector.x = resetValue;

            ImGui::PopStyleColor();

            ImGui::SameLine();
            const bool xPressed = ImGui::DragFloat("##X", &vector.x, speed, 0, 0, "%.1f");
            ImGui::PopItemWidth();

            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button,
                {g_Config.ColorY.x, g_Config.ColorY.y, g_Config.ColorY.z, g_Config.ColorY.w});

            const bool yReset = ImGui::Button("Y", buttonSize);
            if (yReset)
                vector.y = resetValue;

            ImGui::PopStyleColor();

            ImGui::SameLine();
            const bool yPressed = ImGui::DragFloat("##Y", &vector.y, speed, 0, 0, "%.1f");
            ImGui::PopItemWidth();
            
            return xPressed || yPressed || xReset || yReset;
        });
        
        return DrawProperty(name, 2, drawProperty);
    }

    bool DrawColorProperty(const char* name, Vec4& color)
    {
        Delegate<bool()> drawProperty([&]() -> bool
        {
            float floatColor[] = { color.r, color.g, color.b, color.a };
            const bool res = ImGui::ColorEdit4("", floatColor, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs);
            color = { floatColor[0], floatColor[1], floatColor[2], floatColor[3] };
            ImGui::PopItemWidth();
            return res;
        });
        
        return DrawProperty(name, 1, drawProperty);
    }

    bool DrawIntProperty(const char* name, int& num, float speed)
    {
        Delegate<bool()> drawProperty([&]() -> bool
        {
            const bool res =ImGui::DragInt("", &num, speed);
            ImGui::PopItemWidth();
            return res;
        });
        return DrawProperty(name, 1, drawProperty);
    }

    bool DrawInt32Property(const char* name, int32_t& num, float speed)
    {
        int i = static_cast<int>(num);
        Delegate<bool()> drawProperty([&]() -> bool
        {
            const bool r = ImGui::DragInt("", &i, speed);
            ImGui::PopItemWidth();
            return r;
        });
        num = i;
        return DrawProperty(name, 1, drawProperty);
    }

    bool DrawBoolProperty(const char* name, bool& check)
    {
        Delegate<bool()> drawProperty([&]() -> bool
        {
            const bool res = ImGui::Checkbox("", &check);
            ImGui::PopItemWidth();
            return res;
        });
        return DrawProperty(name, 1, drawProperty);
    }

    bool DrawFloatProperty(const char* name, float& num, const float speed)
    {
        Delegate<bool()> drawProperty([&]() -> bool
        {
            const bool res = ImGui::DragFloat("", &num, speed);
            ImGui::PopItemWidth();
            return res;
        });
        return DrawProperty(name, 1, drawProperty);
    }

    bool DrawTextProperty(const char* name, std::string& text)
    {
        Delegate<bool()> drawProperty([&]() -> bool
        {
            const bool res = DrawTextField(text);
            ImGui::PopItemWidth();
            return res;
        });
        return DrawProperty(name, 1, drawProperty);
    }

    bool DrawTextField(std::string& text)
    {
        char nameBuffer[300];
        strcpy_s(nameBuffer, text.c_str());
        if (ImGui::InputText(" ", nameBuffer, sizeof(nameBuffer)))
        {
            if (std::string(nameBuffer).empty())
                text = " ";
            
            text = std::string(nameBuffer);
            return true;
        }
        return false;
    }
}
#endif