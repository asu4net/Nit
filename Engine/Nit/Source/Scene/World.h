#pragma once
#include "Scene.h"
#include "Core/EngineLayer.h"

namespace Nit
{
    class World : public EngineLayer
    {
    public:
        void OnStart() override;
        void OnUpdate(const TimeStep& timeStep) override;
        void OnFixedUpdate(const TimeStep& timeStep) override;
        void OnFinish() override;

    private:
        Shared<Scene> m_ActiveScene;
    }; 
}