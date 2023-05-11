#pragma once
#include "Scene.h"
#include "Core/Time.h"

namespace Nit
{
    class SceneSystem
    {
    protected:
        SceneSystem() = default;
        
        SceneSystem(const Weak<Scene>& scene, const Weak<entt::registry>& registry)
            : m_Scene(scene)
            , m_Registry(registry)
        {}
        
        Scene& GetScene() const
        {
            assert(!m_Scene.expired());
            return *m_Scene.lock();
        }
        
        entt::registry& Registry() const
        {
            assert(!m_Registry.expired());
            return *m_Registry.lock();
        }

        virtual int GetPriority() const { return 0; }
        
        virtual void OnStart() {}
        virtual void OnUpdate(const TimeStep& timeStep) {}
        virtual void OnFixedUpdate(const TimeStep& timeStep) {}
        virtual void OnFinish() {}
        
    private:
        Weak<Scene> m_Scene;
        Weak<entt::registry> m_Registry;

        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND

        friend class Scene;
    };
}