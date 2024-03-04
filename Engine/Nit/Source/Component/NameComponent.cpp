#include "NameComponent.h"
#include "Entity/Entity.h"
#include "ComponentReflection.h"

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<NameComponent>("NameComponent")
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("Name", &NameComponent::Name);

    ComponentReflection::RegisterComponentType<NameComponent>();
}

namespace Nit
{
    NIT_FORCE_LINK_IMPL(NameComponent)
}