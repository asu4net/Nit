#include "ScriptComponent.h"
#include "ComponentReflection.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(ScriptComponent)
}

RTTR_REGISTRATION
{
    using namespace Nit;
    
    rttr::registration::class_<ScriptComponent>("ScriptComponent")
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("ScriptType", &ScriptComponent::ScriptType)
        .property("ExecutionOrder", &ScriptComponent::ExecutionOrder);

    ComponentReflection::RegisterComponentType<ScriptComponent>();
}