#pragma once
#include "Actor.h"
#include "SceneRenderer.h"
#include "SceneSerializer.h"
#include "Core/Time.h"

//TODO: Implement get actor by name

namespace Nit
{
    struct ComponentMetaData
    {
        Delegate<bool(Actor)> HasFunction;
        Delegate<rttr::instance(Actor)> GetByCopyFunction;
        Delegate<void(Actor, const rttr::instance&)> AddFunction;
    };
    
    class SceneSystem;
    
    class Scene : public Asset
    {
    public:
        inline static std::map<rttr::type, ComponentMetaData> ComponentMetaData;

        Scene() = default;
        Scene(const std::string& name, const std::string& path, const Id& id);
        
        bool Load() override;
        void Initialize() override;
        bool Unload() override;
        
        void Start();
        void Update(const TimeStep& timeStep);
        void FixedUpdate(const TimeStep& timeStep);
        void Draw();
        void Finish();

        Weak<entt::registry> GetRegistry() const { return m_Registry; }
        
        void SetRuntimeEnabled(const bool bRuntimeEnabled) { m_bRuntimeEnabled = bRuntimeEnabled; }
        bool IsRuntimeEnabled() const { return m_bRuntimeEnabled; }
        
        int IsStarterScene() const { return m_bIsStarterScene; }
        
        void Save();
        
        void SetWeakPtr(const Weak<Scene>& weakPtr) { m_WeakPtr = weakPtr; }
        const Weak<Scene>& GetWeakPtr() const { return m_WeakPtr; }

        SceneRenderer& GetSceneRenderer() { return m_SceneRenderer; }
        
        Actor CreateActorWithId(Id id, const std::string& name = "Actor",
            const Vec3& position = VecZero, const Quat& rotation = QuatIdentity, const Vec3& scale = VecOne);
        
        Actor CreateActor(const std::string& name = "Actor",
            const Vec3& position = VecZero, const Quat& rotation = QuatIdentity, const Vec3& scale = VecOne);
        
        void DestroyActor(const Actor& actor);
        
    private:
        bool m_bIsStarterScene = false;
        std::string m_SceneData;
        SceneRenderer m_SceneRenderer;
        SceneSerializer m_SceneSerializer;
        std::vector<Shared<SceneSystem>> m_Systems;
        Shared<entt::registry> m_Registry;
        Weak<Scene> m_WeakPtr;
        std::unordered_map<Id, entt::entity> m_IdEntityMap;
        bool m_bRuntimeEnabled = true;

        RTTR_ENABLE(Asset)
        RTTR_REGISTRATION_FRIEND
        
        friend class SceneSerializer;
    };
}
