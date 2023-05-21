#include "DetailsComponent.h"
#include "Scene/Scene.h"

namespace Nit { NIT_FORCE_LINK_IMPL(DetailsComponent) }

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;
    
    registration::class_<DetailsComponent>("DetailsComponent")
        .constructor<>()
        .property("Name", &DetailsComponent::Name)
        .property("Tag", &DetailsComponent::Tag)
        .property("IsSerializable", &DetailsComponent::bIsSerializable)
        .property("Id", &DetailsComponent::m_Id);
    
    NIT_REGISTRY_COMPONENT(DetailsComponent)
}