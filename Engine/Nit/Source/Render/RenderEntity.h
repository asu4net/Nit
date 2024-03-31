#pragma once

namespace Nit
{
    class RenderEntity
    {
    private:
        RawEntity m_RawEntity = entt::null;
        Registry* m_Registry = nullptr;
        
    public:
        RenderEntity() = default;
        RenderEntity(const RawEntity rawEntity);
        RenderEntity(const RenderEntity& other) = default;
        
        RawEntity GetRaw() const { return m_RawEntity; }
        const Matrix4& GetTransform() const;
        void SetTransform(const Matrix4& transform);
        bool IsVisible() const;
        void SetVisible(bool bIsVisible);
        
        bool operator==(const RenderEntity& other) const
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
            NIT_CHECK(IsValid(), "Invalid RenderEntity!");
            return m_Registry->try_get<T>(m_RawEntity);
        }

        template<typename T, typename... Args>
        T& Add(Args&&... args) const
        {
            NIT_CHECK(IsValid(), "Invalid RenderEntity!");
            NIT_CHECK(!Has<T>(), "Entity already has this component!");
            T& component = m_Registry->emplace<T>(m_RawEntity, std::forward<Args>(args)...);
            return component;
        }

        template<typename T>
        T& Get() const
        {
            NIT_CHECK(IsValid(), "Invalid RenderEntity!");
            return m_Registry->get<T>(m_RawEntity);
        }
        
        template<typename T>
        void Remove() const
        {
            NIT_CHECK(IsValid(), "Invalid RenderEntity!");
            m_Registry->erase<T>(m_RawEntity);
        }
    };
}
