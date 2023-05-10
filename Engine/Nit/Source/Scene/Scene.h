#pragma once

namespace Nit
{
    class Scene
    {
    public:
        void Start();
        void Update();
        void Finish();
        
    private:
        Shared<entt::registry> m_Registry;
        std::unordered_map<Id, entt::entity> m_IdEntityMap;
    };
}