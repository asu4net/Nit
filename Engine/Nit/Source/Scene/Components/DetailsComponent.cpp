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
    
    Scene::ComponentMetaData[type::get<DetailsComponent>()] = {
        
        Delegate<bool(Actor)>([](const Actor& actor) -> bool
        {
            return actor.Has<DetailsComponent>();
        }),
        Delegate<instance(Actor)>([](const Actor& actor) -> instance
        {
            return actor.Get<DetailsComponent>();
        }),
        Delegate<void(Actor)>([](const Actor& actor)
        {
            actor.Add<DetailsComponent>();
        })
    };
}