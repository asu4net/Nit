#pragma once
#include "Asset\AssetRef.h"

namespace Nit
{
    struct TransformComponent;
    struct IDComponent;
    struct NameComponent;

    class Entity
    {
    private:
        RawEntity m_RawEntity = entt::null;
        Registry* m_Registry;

    public:
        Entity() = default;
        Entity(const RawEntity rawEntity);
        Entity(const RawEntity rawEntity, Registry* registry);
        Entity(const Entity& other) = default;
        
        RawEntity GetRaw() const { return m_RawEntity; }
        
        TransformComponent& GetTransform() const;
        IDComponent& GetID() const;
        NameComponent& GetName() const;

        bool operator==(const Entity& other) const
        {
            return m_RawEntity == other.m_RawEntity;
        }

        void Reset()
        {
            m_RawEntity = entt::null;
        }

        bool IsValid() const
        {
            if (!m_Registry)
                return false;
            return m_Registry->valid(m_RawEntity);
        }

        template<typename T>
        bool Has() const
        {
            NIT_CHECK(IsValid(), "Invalid Actor!");
            return m_Registry->try_get<T>(m_RawEntity);
        }

        template<typename T, typename... Args>
        T& Add(Args&&... args) const
        {
            NIT_CHECK(IsValid(), "Invalid Actor!");
            NIT_CHECK(!Has<T>(), "Actor already has this component!");
            T& component = m_Registry->emplace<T>(m_RawEntity, std::forward<Args>(args)...);
            return component;
        }

        template<typename T>
        T& Get() const
        {
            NIT_CHECK(IsValid(), "Invalid Actor!");
            return m_Registry->get<T>(m_RawEntity);
        }

        template<typename T>
        void Remove() const
        {
            // Check with strings if type is transform o information component
            NIT_CHECK(IsValid(), "Invalid Actor!");
            m_Registry->erase<T>(m_RawEntity);
        }
    };
}