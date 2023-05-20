#pragma once
#include "Scene.h"
#include "Core/EngineLayer.h"

namespace Nit
{
    class World : public EngineLayer
    {
    public:

        static Weak<Scene> GetActiveScene() { return m_ActiveScene; } 
        
        void OnStart() override;
        void OnUpdate(const TimeStep& timeStep) override;
        void OnFixedUpdate(const TimeStep& timeStep) override;
        void OnPostFixedUpdate(const TimeStep& timeStep) override;
        void OnFinish() override;
    
    private:
        inline static Shared<Scene> m_ActiveScene;
    }; 
}