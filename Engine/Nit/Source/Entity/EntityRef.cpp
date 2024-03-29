#include "EntityRef.h"
#include "Component/IDComponent.h"
#include "Entity/World.h"

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<EntityRef>("EntityRef")
        .constructor<>()(rttr::policy::ctor::as_object)
        .constructor<Id>()
        .property("EntityId", &EntityRef::m_EntityId);
}

namespace Nit
{
    EntityRef::EntityRef()
    {
        Retarget();
    }

    EntityRef::EntityRef(Id id)
    {
        Retarget(id);
    }

    EntityRef::EntityRef(Entity entity)
        : m_TargetEntity(entity)
        , m_EntityId(m_TargetEntity.IsValid() ? m_TargetEntity.Get<IDComponent>().ID : 0)
    {
    }

    EntityRef& EntityRef::operator=(EntityRef&& other) noexcept
    {
        m_TargetEntity = std::move(other.m_TargetEntity);
        m_EntityId = other.m_EntityId;
        return *this;
    }

    bool EntityRef::operator==(const EntityRef& other) const
    {
        return m_EntityId == other.m_EntityId;
    }

    bool EntityRef::operator!=(const EntityRef& other) const
    {
        return !(operator==(other));
    }

    void EntityRef::Retarget()
    {
        Retarget(m_EntityId);
    }

    void EntityRef::Retarget(Id id)
    {
        if (id == 0)
            return;
        
        m_TargetEntity = World::FindEntity(id);
        m_EntityId = m_TargetEntity.IsValid() ? id : 0;
    }

    void EntityRef::Clear()
    {
        m_EntityId = 0;
        m_TargetEntity = Entity();
    }
}
