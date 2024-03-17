#include "CircleComponent.h"
#include "Entity/Entity.h"
#include "ComponentReflection.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(CircleComponent);
}

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<CircleComponent>("CircleComponent")
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("IsVisible", &CircleComponent::IsVisible)
        .property("Radius", &CircleComponent::Radius)
        .property("TintColor", &CircleComponent::TintColor)
        .property("Thickness", &CircleComponent::Thickness)
        .property("Fade", &CircleComponent::Fade)
        .property("SortingLayer", &CircleComponent::SortingLayer);

    ComponentReflection::RegisterComponentType<CircleComponent>();
}