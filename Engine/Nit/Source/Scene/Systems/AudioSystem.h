#pragma once
#include "SceneSystem.h"

namespace Nit
{
    class AudioSystem : public SceneSystem
    {
        NIT_EXECUTION_ORDER(700)
    public:
        AudioSystem() = default;
        AudioSystem(const Weak<Scene>& scene);
        
        void OnUpdate(const TimeStep& timeStep) override;
        
    private:
        void OnAudioSourceComponentAdded(entt::registry&, entt::entity entity);
        void OnAudioSourceComponentRemoved(entt::registry&, entt::entity entity);
        
        RTTR_ENABLE(SceneSystem)
    };
    NIT_FORCE_LINK(AudioSystem)
}
