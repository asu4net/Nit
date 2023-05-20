#pragma once
#include "SceneSystem.h"
#include "Scene/Actor.h"

namespace Nit
{
    class CameraSystem : public SceneSystem
    {
        NIT_EXECUTION_ORDER(100)
    public:
        CameraSystem() = default;
        CameraSystem(const Weak<Scene>& scene);

        static Actor GetMainCameraActor() { return m_MainCameraActor; }
        
        void OnUpdate(const TimeStep& timeStep) override;
        
    private:
        inline static Actor m_MainCameraActor;
        RTTR_ENABLE(SceneSystem)
    };
    NIT_FORCE_LINK(CameraSystem)
}