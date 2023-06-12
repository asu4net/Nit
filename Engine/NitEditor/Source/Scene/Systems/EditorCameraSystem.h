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

        static Actor GetEditorCameraActor() { return m_EditorCameraActor; }

        void OnUpdate(const TimeStep& timeStep) override;

        void MoveOrthographicCamera(TransformComponent& transform,
            EditorCameraComponent& editorCamera, float deltaTime);

        void MovePerspectiveCamera(TransformComponent& transform,
            EditorCameraComponent& editorCamera, float deltaTime);
        
    private:
        bool m_bMouseDown = false;
        Vec3 m_AuxPosition = VecZero;
        Vec3 m_Offset = VecZero;
        Camera m_AuxCamera;
        Vec2 m_LastMousePos = VecZero;
        
        inline static Actor m_EditorCameraActor;
        RTTR_ENABLE(SceneSystem)
    };
    NIT_FORCE_LINK(EditorCameraSystem);
}