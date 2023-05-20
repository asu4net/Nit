#include "SceneSystem.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::class_<SceneSystem>("SceneSystem")
        .constructor<>()
        .constructor<const Weak<Scene>&>()
        .property("m_Scene", &SceneSystem::m_Scene);
}

namespace Nit
{
    SceneSystem::SceneSystem(const Weak<Scene>& scene)
            : m_Scene(scene)
    {}

    Scene& SceneSystem::GetScene() const
    {
        assert(!m_Scene.expired());
        return *m_Scene.lock();
    }

    entt::registry& SceneSystem::Registry() const
    {
        const auto registry = RegistryPtr();
        assert(!registry.expired());
        return *registry.lock();
    }

    Weak<entt::registry> SceneSystem::RegistryPtr() const
    {
        assert(!m_Scene.expired());
        Weak<entt::registry> registry = m_Scene.lock()->GetRegistry();
        return registry;
    }
}
