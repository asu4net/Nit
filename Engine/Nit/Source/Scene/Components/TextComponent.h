#pragma once
#include "ActorComponent.h"
#include "Core/Asset/AssetLink.h"
#include "Rendering/Text/Font.h"

namespace Nit
{
    struct TextComponent : ActorComponent
    {
        TextComponent() = default;
        TextComponent(const std::string& text)
            : Text(text)
        {}
        
        std::string Text = "Text";
        Vec4 Color = White;
        AssetLink<Font> Font;
        Vec2 Size = VecOne;
        float Spacing =  0.9f;

        RTTR_ENABLE(ActorComponent)
    };
    NIT_FORCE_LINK(TextComponent)
}