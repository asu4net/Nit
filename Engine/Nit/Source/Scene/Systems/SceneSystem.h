#pragma once
#include "Scene/Scene.h"
#include "Core/Time.h"

namespace Nit
{
    class SceneSystem
    {
    protected:
        SceneSystem() = default;
        SceneSystem(const Weak<Scene>& scene);
        Scene& GetScene() const;
        entt::registry& Registry() const;
        Weak<entt::registry> RegistryPtr() const;
         
        virtual int GetExecutionOrder() const { return 10000; }
        
        virtual void OnStart() {}
        virtual void OnUpdate(const TimeStep& timeStep) {}
        virtual void OnFixedUpdate(const TimeStep& timeStep) {}
        virtual void OnFinish() {}
    
    private:
        Weak<Scene> m_Scene;
        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND
        friend class Scene;
    };
}
