#include "TextComponent.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::class_<TextComponent>("TextComponent")
        .property("Text", &TextComponent::Text)
        .property("Color", &TextComponent::Color)
        .property("Font", &TextComponent::Font)
        .property("Size", &TextComponent::Size)
        .property("Spacing", &TextComponent::Spacing);
}