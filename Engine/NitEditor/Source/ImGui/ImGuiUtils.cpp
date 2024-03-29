#include "ImGuiUtils.h"
#include <imgui.h>
#include "imgui_internal.h"
#include "Asset\AssetRef.h"
#include "Asset\Content.h"
#include "Component/IDComponent.h"
#include "Component/NameComponent.h"
#include "Core\File.h"
#include "Entity/Entity.h"
#include "Entity/EntityRef.h"
#include "Entity/World.h"

namespace ImGui
{
    using namespace Nit;

    PropertyStyles::PropertyStyles()
        : ColumnWidth(120)
        , ItemSpacing(0, 6)
        , ColorX(1, 0.3f, 0.3f, 1)
        , ColorY(0.368f, 0.737f, 0.521f, 1)
        , ColorZ(0.3f, 0.3f, 1, 1)
        , ColorW(0.43f, 0.258f, 0.96f, 1)
    {
    }
    
    PropertyStyles Styles;

    void SetPropertyStyles(const PropertyStyles& propertyStyles) { Styles = propertyStyles; }
    PropertyStyles GetPropertyStyles() { return Styles; }

    bool bInPropertyContext = false;

    void BeginProperty(const char* label, uint32_t items)
    {
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, Styles.ColumnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(items, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{
            Styles.ItemSpacing.x, Styles.ItemSpacing.y });

        bInPropertyContext = true;
    }

    void EndProperty()
    {
        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();

        bInPropertyContext = false;
    }

    void Spacing(uint32_t spacing)
    {
        for (uint32_t i = 0; i < spacing; i++)
            ImGui::Spacing();
    }

    bool CenteredButton(const char* label, float alignment)
    {
        const ImGuiStyle& style = ImGui::GetStyle();

        const float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
        const float avail = ImGui::GetContentRegionAvail().x;

        const float off = (avail - size) * alignment;
        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

        return ImGui::Button(label);
    }

    bool InputText(const char* label, String& text)
    {
        BeginProperty(label);
        static constexpr uint32_t MAX_CHARS = 300;
        char textBuffer[MAX_CHARS];
        strcpy_s(textBuffer, text.c_str());
        const bool bTextChanged = ImGui::InputText("##text", textBuffer, sizeof(textBuffer));
        if (bTextChanged)
        {
            text = textBuffer;
        }
        EndProperty();
        return bTextChanged;
    }

    bool InputFolder(const char* label, Nit::String& text)
    {
        BeginProperty(label);
        static constexpr uint32_t MAX_CHARS = 300;
        char textBuffer[MAX_CHARS];
        strcpy_s(textBuffer, text.c_str());
        bool bTextChanged = ImGui::InputText("##text", textBuffer, sizeof(textBuffer));
        ImGui::SameLine();

        if (ImGui::Button("Find"))
        {
            const String path = File::SelectFolder("", text);

            if (!path.empty())
            {
                strcpy_s(textBuffer, path.c_str());
                bTextChanged = true;
            }
        }

        if (bTextChanged)
        {
            text = textBuffer;
        }

        EndProperty();
        return bTextChanged;
    }

    void Text(const char* label, const char* text, ...)
    {
        BeginProperty(label);
        va_list args;
        va_start(args, text);
        ImGui::Text(text);
        va_end(args);
        EndProperty();
    }

    bool Bool(const char* label, bool& bEnabled)
    {
        BeginProperty(label);
        bool bChanged = ImGui::Checkbox("##", &bEnabled);
        if (bInPropertyContext)
            ImGui::PopItemWidth();
        EndProperty();
        return bChanged;
    }

    bool DragSInt(const char* label, int& num, float speed /*= 1.f*/)
    {
        BeginProperty(label);
        bool bChanged = ImGui::DragInt("##", &num, speed);
        if (bInPropertyContext)
            ImGui::PopItemWidth();
        EndProperty();
        return bChanged;
    }

    bool DragSInt32(const char* label, int32_t& num, float speed /*= 1.f*/)
    {
        BeginProperty(label);
        bool bChanged = ImGui::DragInt("##", &num, speed);
        if (bInPropertyContext)
            ImGui::PopItemWidth();
        EndProperty();
        return bChanged;
    }

    bool DragUInt32(const char* label, uint32_t& num, float speed /*= 1.f*/)
    {
        BeginProperty(label);
        int i = static_cast<int>(num);
        bool bChanged = ImGui::DragInt("##", &i, speed);
        num = i;
        if (bInPropertyContext)
            ImGui::PopItemWidth();
        EndProperty();
        return bChanged;
    }

    bool DragFloat32(const char* label, float& num, float speed /*= 0.1f*/)
    {
        BeginProperty(label);
        bool bChanged = ImGui::DragFloat("##", &num, speed);
        if (bInPropertyContext)
            ImGui::PopItemWidth();
        EndProperty();
        return bChanged;
    }

    void Combo(const char* label, String& selected, const DynamicArray<String>& options)
    {
        BeginProperty(label);
        if (selected.empty())
            selected = options[0];

        if (ImGui::BeginCombo("##combo", selected.c_str()))
        {
            for (const String& option : options)
            {
                const bool isSelected = selected == option;
                if (ImGui::Selectable(option.c_str()))
                {
                    selected = option;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }
        EndProperty();
    }

    bool DragFloatWithButton(const char* label, float& value, const Color& resetColor, float resetValue, float speed)
    {
        ImGui::PushID(label);
        const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        const ImVec2 buttonSize = { lineHeight + 3.f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, { resetColor.r, resetColor.g, resetColor.b, resetColor.a });

        const bool bReset = ImGui::Button(label, buttonSize);

        ImGui::PopStyleColor();
        ImGui::SameLine();
        const bool bChanged = ImGui::DragFloat("##value", &value, speed);
        
        if (bReset)
            value = resetValue;

        if (bInPropertyContext)
            ImGui::PopItemWidth();
        
        ImGui::PopID();

        return bChanged || bReset;
    }

    bool DragVector2(const char* label, Nit::Vector2& vector, const Nit::Vector2& resetValue, float speed)
    {
        BeginProperty(label, 2);
        bool bXChanged = DragFloatWithButton("X", vector.x, Styles.ColorX, resetValue.x, speed);
        ImGui::SameLine();
        bool bYChanged = DragFloatWithButton("Y", vector.y, Styles.ColorY, resetValue.y, speed);
        EndProperty();
        return bXChanged || bYChanged;
    }

    bool DragVector3(const char* label, Nit::Vector3& vector, const Nit::Vector3& resetValue /*= Nit::Vector3::Zero*/, float speed /*= 0.05f*/)
    {
        BeginProperty(label, 3);
        bool bXChanged = DragFloatWithButton("X", vector.x, Styles.ColorX, resetValue.x, speed);
        ImGui::SameLine();
        bool bYChanged = DragFloatWithButton("Y", vector.y, Styles.ColorY, resetValue.y, speed);
        ImGui::SameLine();
        bool bZChanged = DragFloatWithButton("Z", vector.z, Styles.ColorZ, resetValue.z, speed);
        EndProperty();
        return bXChanged || bYChanged || bZChanged;
    }

    bool DragVector4(const char* label, Nit::Vector4& vector, const Nit::Vector4& resetValue /*= Nit::Vector4::Zero*/, float speed /*= 0.05f*/)
    {
        BeginProperty(label, 4);
        bool bXChanged = DragFloatWithButton("X", vector.x, Styles.ColorX, resetValue.x, speed);
        ImGui::SameLine();
        bool bYChanged = DragFloatWithButton("Y", vector.y, Styles.ColorY, resetValue.y, speed);
        ImGui::SameLine();
        bool bZChanged = DragFloatWithButton("Z", vector.z, Styles.ColorZ, resetValue.z, speed);
        ImGui::SameLine();
        bool bWChanged = DragFloatWithButton("W", vector.w, Styles.ColorW, resetValue.w, speed);
        EndProperty();
        return bXChanged || bYChanged || bZChanged || bWChanged;
    }

    bool ColorPalette(const char* label, Nit::Color& color)
    {
        BeginProperty(label);
        float floatColor[] = { color.r, color.g, color.b, color.a };
        const bool bChanged = ImGui::ColorEdit4("", floatColor, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs);
        color = { floatColor[0], floatColor[1], floatColor[2], floatColor[3] };
        EndProperty();
        return bChanged;
    }

    void Property(const Nit::Instance& instance)
    {
        const Type& type = instance.get_type();

        auto properties = type.get_properties();

        for (auto& property : properties)
        {
            Variant variant = property.get_value(instance);

            if (!variant.is_valid())
                continue;

            const String propertyName = property.get_name().to_string();
            const Type propertyType = property.get_type();

            if (propertyType == Type::get<Vector2>())
            {
                Vector2 propValue = variant.get_value<Vector2>();

                if (ImGui::DragVector2(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == Type::get<Vector3>())
            {
                Vector3 propValue = variant.get_value<Vector3>();

                if (ImGui::DragVector3(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == Type::get<Vector4>())
            {
                Vector4 propValue = variant.get_value<Vector4>();

                if (ImGui::DragVector4(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == Type::get<Color>())
            {
                Color propValue = variant.get_value<Color>();

                if (ImGui::ColorPalette(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == Type::get<int>())
            {
                int propValue = variant.get_value<int>();

                if (ImGui::DragSInt(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == Type::get<float>())
            {
                float propValue = variant.get_value<float>();

                if (ImGui::DragFloat32(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == Type::get<bool>())
            {
                bool propValue = variant.get_value<bool>();

                if (ImGui::Bool(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == Type::get<int32_t>())
            {
                int32_t propValue = variant.get_value<int32_t>();

                if (ImGui::DragSInt32(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == Type::get<uint32_t>())
            {
                uint32_t propValue = variant.get_value<uint32_t>();

                if (ImGui::DragUInt32(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType == Type::get<String>())
            {
                String propValue = variant.get_value<String>();

                if (ImGui::InputText(propertyName.c_str(), propValue))
                {
                    property.set_value(instance, propValue);
                }
                continue;
            }

            if (propertyType.is_valid() && propertyType.is_enumeration())
            {
                rttr::enumeration propEnum = propertyType.get_enumeration();
                if (!propEnum.is_valid())
                    continue;

                auto enumNames = propEnum.get_names();
                DynamicArray<String> enumNamesStr;

                for (const auto& enumName : enumNames)
                {
                    enumNamesStr.push_back(enumName.to_string());
                }

                String selectedEnum = variant.to_string();
                ImGui::Combo(propertyName.c_str(), selectedEnum, enumNamesStr);
                Variant enumValue = propEnum.name_to_value(selectedEnum);
                property.set_value(instance, enumValue);
                continue;
            }

            if (propertyType == Type::get<Id>())
            {
                //TODO
                continue;
            }

            if (propertyType == Type::get<AssetRef>())
            {
                DynamicArray<String> assetNames;
                assetNames.push_back("None");
                Content::EachAsset({
                    [&assetNames](const AssetRef& asset) {
                        assetNames.push_back(asset.Get()->GetAssetData().Name);
                    }
                });

                AssetRef ref = variant.get_value<AssetRef>();
                String selectedAsset = !ref.IsValid() ? "None" : ref.Get()->GetAssetData().Name;
                ImGui::Combo(propertyName.c_str(), selectedAsset, assetNames);
                AssetRef assetRef = Content::GetAssetByName(selectedAsset);
                property.set_value(instance, assetRef);
                continue;
            }

            if (propertyType == Type::get<EntityRef>())
            {
                DynamicArray<String> entityNames;
                entityNames.push_back("None");
                World::EachEntity({
                [&entityNames](const Entity& entity)
                {
                    entityNames.push_back(entity.GetName().Name);
                }});

                EntityRef ref = variant.get_value<EntityRef>();

                String selectedEntityName = !ref.IsValid() ? "None" : ref.Get().GetName().Name;
                ImGui::Combo(propertyName.c_str(), selectedEntityName, entityNames);
                property.set_value(instance, World::FindEntityByName(selectedEntityName));
                
                continue;
            }

            if (propertyType.is_class())
            {
                Class(variant);
                property.set_value(instance, variant);
            }
        }
    }

    void Class(const Nit::Instance& instance, bool drawAsTree)
    {
        if (!instance.is_valid())
            return;

        if (!drawAsTree)
        {
            Property(instance);
            return;
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);

        Type type = instance.get_type();

        if (ImGui::TreeNodeEx(type.get_name().to_string().c_str()))
        {
            ImGui::Spacing(3);
            Property(instance);
            ImGui::Spacing(4);
            ImGui::TreePop();
        }
    }
}
