#include "CircleColliderComponent.h"
#include "ComponentReflection.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(CircleColliderComponent)
}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Nit;
    
    registration::class_<CircleColliderComponent>("CircleColliderComponent")
            .constructor<>()(policy::ctor::as_object)
            .property("Radius",            &CircleColliderComponent::Radius)
            .property("Center",            &CircleColliderComponent::Center)
            .property("PhysicMaterialRef", &CircleColliderComponent::PhysicMaterialRef);

    ComponentReflection::RegisterComponentType<CircleColliderComponent>();
}