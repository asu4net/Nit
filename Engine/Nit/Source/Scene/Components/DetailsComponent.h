#pragma once
#include "ActorComponent.h"

namespace Nit
{
    struct DetailsComponent : ActorComponent
    {
        std::string Name;
        std::string Tag;
        bool bIsSerializable = true;
        
        DetailsComponent()
            : Name("None")
            , Tag("None")
        {
        }
        
        DetailsComponent(const std::string& name, const std::string& tag, const Id& id,
            const bool bIsSerializable)
            : Name(name)
            , Tag(tag)
            , bIsSerializable(bIsSerializable)
            , m_Id(id)
        {}

        Id GetId() const { return m_Id; }
        
    private:
        Id m_Id{0};
        
        RTTR_ENABLE(ActorComponent)
        RTTR_REGISTRATION_FRIEND
    };
    
    NIT_FORCE_LINK(DetailsComponent)
}
