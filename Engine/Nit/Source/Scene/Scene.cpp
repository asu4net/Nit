#include "Scene.h"
#include "SceneSystem.h"

namespace Nit
{
    void Scene::Start()
    {
        auto derivedSystems = rttr::type::get<SceneSystem>().get_derived_classes();
        m_Systems.reserve(derivedSystems.size());
        for (auto& systemType : derivedSystems)
        {
            rttr::variant systemInstance = systemType.create({m_WeakPtr, m_Registry});
            if (!systemInstance.is_valid())
                continue;
            m_Systems.push_back(systemInstance.get_value<Shared<SceneSystem>>());
        }
    }

    void Scene::Update(const TimeStep& timeStep)
    {
    }

    void Scene::FixedUpdate(const TimeStep& timeStep)
    {
    }

    void Scene::Finish()
    {
    }
}
