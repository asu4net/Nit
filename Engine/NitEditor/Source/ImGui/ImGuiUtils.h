#pragma once

namespace ImGui
{
    //TODO Change const char*, use String instead

    struct PropertyStyles
    {
        float ColumnWidth;
        Nit::Vector2 ItemSpacing;
        Nit::Color ColorX;
        Nit::Color ColorY;
        Nit::Color ColorZ;
        Nit::Color ColorW;

        PropertyStyles();
    };

    void SetPropertyStyles(const PropertyStyles& propertyStyles);
    PropertyStyles GetPropertyStyles();

    void BeginProperty(const char* label, uint32_t items = 1);
    void EndProperty();

    void Spacing(uint32_t spacing);
    bool CenteredButton(const char* label, float alignment = 0.5f);

    bool InputText(const char* label, Nit::String& text);
    bool InputFolder(const char* label, Nit::String& text);
    
    void Text(const char* label, const char* text, ...);
    bool Bool(const char* label, bool& bEnabled);
    bool DragSInt(const char* label, int& num, float speed = 1.f);
    bool DragSInt32(const char* label, int32_t& num, float speed = 1.f);
    bool DragUInt32(const char* label, uint32_t& num, float speed = 1.f);
    bool DragFloat32(const char* label, float& num, float speed = 0.1f);
    void Combo(const char* label, Nit::String& selected, const Nit::DynamicArray<Nit::String>& options);
    bool DragFloatWithButton(const char* label, float& value, const Nit::Color& resetColor = Nit::Color::LightRed, 
        float resetValue = 0.f, float speed = 0.05f);
    bool DragVector2(const char* label, Nit::Vector2& vector, const Nit::Vector2& resetValue = Nit::Vector2::Zero, float speed = 0.05f);
    bool DragVector3(const char* label, Nit::Vector3& vector, const Nit::Vector3& resetValue = Nit::Vector3::Zero, float speed = 0.05f);
    bool DragVector4(const char* label, Nit::Vector4& vector, const Nit::Vector4& resetValue = Nit::Vector4::Zero, float speed = 0.05f);
    bool ColorPalette(const char* label, Nit::Color& color);
    
    void Property(const Nit::Instance& instance);
    void Class(const Nit::Instance& instance, bool drawAsTree = true);
}