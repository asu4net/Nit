#include "TransformComponent.h"
#include "ComponentReflection.h"

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<TransformComponent>("TransformComponent")
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("Position", &TransformComponent::Position)
        .property("Rotation", &TransformComponent::Rotation)
        .property("Scale", &TransformComponent::Scale);

    ComponentReflection::RegisterComponentType<TransformComponent>();
}

namespace Nit
{
    NIT_FORCE_LINK_IMPL(TransformComponent)
}