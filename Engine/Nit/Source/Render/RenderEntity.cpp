#include "NitPCH.h"
#include "RenderEntity.h"
#include "RenderComponents.h"
#include "Renderer.h"

namespace Nit
{
    RenderEntity::RenderEntity(const RawEntity rawEntity)
        : m_Registry(Renderer::GetRegistryPtr())
        , m_RawEntity(rawEntity)
    {
    }

    const Matrix4& RenderEntity::GetTransform() const
    {
        NIT_CHECK(IsValid(), "Invalid RenderEntity!");
        return m_Registry->get<RenderComponent>(m_RawEntity).Transform;
    }

    void RenderEntity::SetTransform(const Matrix4& transform)
    {
        NIT_CHECK(IsValid(), "Invalid RenderEntity!");
        m_Registry->get<RenderComponent>(m_RawEntity).Transform = transform;
    }

    bool RenderEntity::IsVisible() const
    {
        NIT_CHECK(IsValid(), "Invalid RenderEntity!");
        return m_Registry->get<RenderComponent>(m_RawEntity).bIsVisible;
    }

    void RenderEntity::SetVisible(bool bIsVisible)
    {
        NIT_CHECK(IsValid(), "Invalid RenderEntity!");
        m_Registry->get<RenderComponent>(m_RawEntity).bIsVisible = bIsVisible;
    }
}
