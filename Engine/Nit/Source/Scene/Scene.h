#pragma once
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
        void Finish();

        void SetWeakPtr(const Weak<Scene>& weakPtr) { m_WeakPtr = weakPtr; };
        const Weak<Scene>& GetWeakPtr() const { return m_WeakPtr; }
    
    private:
        std::vector<Shared<SceneSystem>> m_Systems;
        Shared<entt::registry> m_Registry;
        Weak<Scene> m_WeakPtr;
        std::unordered_map<Id, entt::entity> m_IdEntityMap;

    };
}
