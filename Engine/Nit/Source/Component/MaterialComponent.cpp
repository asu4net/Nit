#include "MaterialComponent.h"
#include "ComponentReflection.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(MaterialComponent);
}

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<MaterialComponent>("MaterialComponent")
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("MaterialRef", &MaterialComponent::MaterialRef);

    ComponentReflection::RegisterComponentType<MaterialComponent>();
}