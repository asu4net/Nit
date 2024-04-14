#pragma once

namespace ImGui
{
    //TODO Change const char*, use String instead

    struct PropertyStyles
    {
        float ColumnWidth;
        Nit::CVector2 ItemSpacing;
        Nit::CColor ColorX;
        Nit::CColor ColorY;
        Nit::CColor ColorZ;
        Nit::CColor ColorW;

        PropertyStyles();
    };

    void SetPropertyStyles(const PropertyStyles& propertyStyles);
    PropertyStyles GetPropertyStyles();

    void BeginProperty(const char* label, uint32_t items = 1);
    void EndProperty();

    void Spacing(uint32_t spacing);
    bool CenteredButton(const char* label, float alignment = 0.5f);

    bool InputText(const char* label, Nit::TString& text);
    bool InputFolder(const char* label, Nit::TString& text);
    
    void Text(const char* label, const char* text, ...);
    bool Bool(const char* label, bool& bEnabled);
    bool DragSInt(const char* label, int& num, float speed = 1.f);
    bool DragSInt32(const char* label, int32_t& num, float speed = 1.f);
    bool DragUInt32(const char* label, uint32_t& num, float speed = 1.f);
    bool DragFloat32(const char* label, float& num, float speed = 0.1f);
    void Combo(const char* label, Nit::TString& selected, const Nit::TDynamicArray<Nit::TString>& options);
    bool DragFloatWithButton(const char* label, float& value, const Nit::CColor& resetColor = Nit::CColor::LightRed, 
        float resetValue = 0.f, float speed = 0.05f);
    bool DragVector2(const char* label, Nit::CVector2& vector, const Nit::CVector2& resetValue = Nit::CVector2::Zero, float speed = 0.05f);
    bool DragVector3(const char* label, Nit::CVector3& vector, const Nit::CVector3& resetValue = Nit::CVector3::Zero, float speed = 0.05f);
    bool DragVector4(const char* label, Nit::Vector4& vector, const Nit::Vector4& resetValue = Nit::Vector4::Zero, float speed = 0.05f);
    bool ColorPalette(const char* label, Nit::CColor& color);
    
    void Property(const Nit::Instance& instance);
    void Class(const Nit::Instance& instance, bool drawAsTree = true);
}