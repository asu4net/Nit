#pragma once

namespace Nit::ImGuiHelpers
{
    struct PropertyConfiguration
    {
        float ColumnWidth;
        Vec2 ItemSpacing;
        
        Vec4 ColorX;
        Vec4 ColorY;
        Vec4 ColorZ;
    };

    extern PropertyConfiguration g_Config;
    
    bool DrawProperty(const char* name, int items, Delegate<bool()>& drawFieldsDelegate);
    bool DrawVec3Property(const char* name, Vec3& vector, float resetValue = 0, float speed = 0.05f);
    bool DrawVec2Property(const char* name, Vec2& vector, float resetValue = 0, float speed = 0.05f);
    bool DrawColorProperty(const char* name, Vec4& color);
    bool DrawIntProperty(const char* name, int& num, float speed = 1.f);
    bool DrawInt32Property(const char* name, int32_t& num, float speed = 1.f);
    bool DrawUInt32Property(const char* name, uint32_t& num, float speed = 1.f);
    bool DrawBoolProperty(const char* name, bool& check);
    bool DrawFloatProperty(const char* name, float& num, const float speed = 0.1f);
    bool DrawTextProperty(const char* name, std::string& text);
    bool DrawTextField(std::string& text);
    bool DrawEnumProperty(const char* name, std::string& selected, const std::vector<std::string>& options);
    void DrawSpacing(uint32_t spacing = 1);
}