#include "SpriteComponent.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::class_<SpriteComponent>("SpriteComponent")
        .constructor<>()
        .property("TextureLink", &SpriteComponent::TextureLink)
        .property("Color", &SpriteComponent::Color)
        .property("bIsSubTexture", &SpriteComponent::bIsSubTexture)
        .property("SubTextureSize", &SpriteComponent::SubTextureSize)
        .property("LocationInAtlas", &SpriteComponent::LocationInAtlas)
        .property("UVScale", &SpriteComponent::UVScale)
        .property("Size", &SpriteComponent::Size)
        .property("Flip", &SpriteComponent::Flip);
}