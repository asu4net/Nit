#pragma once

namespace Nit
{
    class Actor
    {
    public:
        Actor() = default;
        Actor(const entt::entity entity, const std::weak_ptr<entt::registry>& registry)
            : m_Entity(entity)
            , m_Registry(registry)
        {}
        
        Actor(const Actor& other) = default;
        
        entt::entity GetEntity() const { return m_Entity; }
        
        bool operator==(const Actor& other)
        {
            return m_Entity == other.m_Entity;
        }
        
        bool IsValid() const
        {
            if (m_Registry.expired())
                return false;
            return m_Registry.lock()->valid(m_Entity);
        }
        
        template<typename T>
        bool Has() const
        {
            assert(IsValid());
            return m_Registry.lock()->try_get<T>(m_Entity);
        }

        template<typename T, typename... Args>
        T& Add(Args&&... args) const
        {
            assert(IsValid());
            assert(!Has<T>());
            T& component = m_Registry.lock()->emplace<T>(m_Entity, std::forward<Args>(args)...);
            return component;
        }
        
        template<typename T>
        T& Get() const
        {
            assert(IsValid());
            return m_Registry.lock()->get<T>(m_Entity);
        }
        
        template<typename T>
        void Remove() const
        {
            assert(IsValid());
            m_Registry.lock()->erase<T>(m_Entity);
        }
        
    private:
        entt::entity m_Entity = entt::null;
        std::weak_ptr<entt::registry> m_Registry;
    };
}