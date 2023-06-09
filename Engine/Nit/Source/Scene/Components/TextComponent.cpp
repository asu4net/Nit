﻿#include "TextComponent.h"
#include "Scene/Actor.h"
#include "Scene/Scene.h"

namespace Nit { NIT_FORCE_LINK_IMPL(TextComponent) }

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::class_<TextComponent>("TextComponent")
        .constructor<>()
        .property("Text", &TextComponent::Text)
        .property("Color", &TextComponent::Color)
        .property("Font", &TextComponent::Font)
        .property("Size", &TextComponent::Size)
        .property("Spacing", &TextComponent::Spacing);

    NIT_REGISTRY_COMPONENT(TextComponent)
}