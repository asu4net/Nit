#pragma once
#include "Scene.h"
#include "Core/EngineLayer.h"

namespace Nit
{
    class World : public EngineLayer
    {
    public:

        static Weak<Scene> GetActiveScenePtr() { return m_ActiveScene; } 
        static Scene& GetActiveScene()
        {
            assert(m_ActiveScene);
            return *m_ActiveScene.get();
        }
        
        static bool HasActiveScene()
        {
            return m_ActiveScene.get();
        }
        
        void LoadScene(const std::string& name);
        
        void OnStart() override;
        void OnUpdate(const TimeStep& timeStep) override;
        void OnFixedUpdate(const TimeStep& timeStep) override;
        void OnPostFixedUpdate(const TimeStep& timeStep) override;
        void OnFinish() override;
        
    private:
        inline static Shared<Scene> m_ActiveScene;
        std::unordered_map<std::string, Shared<Scene>> m_Scenes;

        void GetAllSceneAssets();
    }; 
}