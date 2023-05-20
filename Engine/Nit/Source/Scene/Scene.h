#pragma once
#include "Actor.h"
#include "SceneRenderer.h"
#include "Core/Time.h"

namespace Nit
{
    class SceneSystem;
    class Scene
    {
    public:
        void Start();
        void Update(const TimeStep& timeStep);
        void FixedUpdate(const TimeStep& timeStep);
        void Draw();
        void Finish();

        Weak<entt::registry> GetRegistry() const { return m_Registry; }

        void SetRuntimeEnabled(bool bRuntimeEnabled);
        bool IsRuntimeEnabled() const { return m_bRuntimeEnabled; }
        
        void SetWeakPtr(const Weak<Scene>& weakPtr) { m_WeakPtr = weakPtr; }
        const Weak<Scene>& GetWeakPtr() const { return m_WeakPtr; }

        SceneRenderer& GetSceneRenderer() { return m_SceneRenderer; }
        
        Actor CreateActorWithId(Id id, const std::string& name = "Actor",
            const Vec3& position = VecZero, const Quat& rotation = QuatIdentity, const Vec3& scale = VecOne);
        
        Actor CreateActor(const std::string& name = "Actor",
            const Vec3& position = VecZero, const Quat& rotation = QuatIdentity, const Vec3& scale = VecOne);
        
        void DestroyActor(const Actor& actor);
        
    private:
        SceneRenderer m_SceneRenderer;
        std::vector<Shared<SceneSystem>> m_Systems;
        Shared<entt::registry> m_Registry;
        Weak<Scene> m_WeakPtr;
        std::unordered_map<Id, entt::entity> m_IdEntityMap;
        bool m_bRuntimeEnabled = true;
    };
}
