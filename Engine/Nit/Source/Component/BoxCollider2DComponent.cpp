#include "BoxCollider2DComponent.h"
#include "ComponentReflection.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(BoxCollider2DComponent)
}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Nit;
    
    registration::class_<BoxCollider2DComponent>("BoxCollider2DComponent")
            .constructor<>()(policy::ctor::as_object)
            .property("Size",              &BoxCollider2DComponent::Size)
            .property("Center",            &BoxCollider2DComponent::Center)
            .property("PhysicMaterialRef", &BoxCollider2DComponent::PhysicMaterialRef);

    ComponentReflection::RegisterComponentType<BoxCollider2DComponent>();
}