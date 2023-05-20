#pragma once
#include "Nit.h"

namespace Nit
{
    struct EditorCameraComponent;

    class EditorCameraSystem : public SceneSystem
    {
        NIT_EXECUTION_ORDER(150)
    public:
        EditorCameraSystem() = default;
        EditorCameraSystem(const Weak<Scene>& scene);

        void OnUpdate(const TimeStep& timeStep) override;

        void MoveCamera(TransformComponent& transform,
            const EditorCameraComponent& editorCamera, float deltaTime);
        
    private:
        RTTR_ENABLE(SceneSystem)
    };
    NIT_FORCE_LINK(EditorCameraSystem);
}