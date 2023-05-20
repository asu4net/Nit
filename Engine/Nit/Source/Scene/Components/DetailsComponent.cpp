#include "DetailsComponent.h"

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
}