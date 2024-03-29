#pragma once
#include "Entity.h"

namespace Nit
{
    class EntityRef
    {
    public:
        EntityRef();
        EntityRef(Id id);
        EntityRef(Entity entity);

        EntityRef(const EntityRef& other);
        EntityRef(EntityRef&& other) noexcept;
        ~EntityRef();

        EntityRef& operator = (const EntityRef& other) = default;
        EntityRef& operator = (EntityRef&& other) noexcept;
        bool operator == (const EntityRef& other) const;
        bool operator != (const EntityRef& other) const;
        
        void Retarget();
        void Retarget(Id id);
        void Clear();
        
        bool IsValid() const { return m_TargetEntity.IsValid(); }
        Entity Get()   const { return m_TargetEntity; }
        
    private:
        Id m_EntityId = 0;
        Entity m_TargetEntity;
        
        RTTR_ENABLE_NO_VIRTUAL
        RTTR_REGISTRATION_FRIEND
    };
}


