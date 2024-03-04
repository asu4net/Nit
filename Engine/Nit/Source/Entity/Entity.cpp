#include "Entity.h"
#include "World.h"
#include "Component/TransformComponent.h"
#include "Component/IDComponent.h"
#include "Component/NameComponent.h"

namespace Nit
{
    Entity::Entity(const RawEntity rawEntity)
        : m_RawEntity(rawEntity)
        , m_Registry(World::GetRegistryPtr())
    {
    }

    Entity::Entity(const RawEntity rawEntity, Registry* registry)
        : m_RawEntity(rawEntity)
        , m_Registry(registry)
    {
    }

    TransformComponent& Entity::GetTransform() const
    {
        return Get<TransformComponent>();
    }

    IDComponent& Entity::GetID() const
    {
        return Get<IDComponent>();
    }

    NameComponent& Entity::GetName() const
    {
        return Get<NameComponent>();
    }
}