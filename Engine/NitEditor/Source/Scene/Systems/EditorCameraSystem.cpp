#include "EditorCameraSystem.h"
#include "Scene/Components/EditorCameraComponent.h"

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Nit;

    registration::class_<EditorCameraSystem>("EditorCameraSystem")
        .constructor<>()
        .constructor<const Weak<Scene>&>();
}

namespace Nit
{
    EditorCameraSystem::EditorCameraSystem(const Weak<Scene>& scene)
        : SceneSystem(scene)
    {
    }

    void EditorCameraSystem::OnUpdate(const TimeStep& timeStep)
    {
        if (GetScene().IsRuntimeEnabled()) return;
        
        const auto view = Registry().view<TransformComponent, EditorCameraComponent>();

        view.each([&](TransformComponent& transform, EditorCameraComponent& camera)
        {
            camera.CameraData.CalculateProjectionViewMat4(transform.Position, transform.Rotation);
            GetScene().GetSceneRenderer().PushCamera(camera.CameraData);
            MoveCamera(transform, camera, timeStep.DeltaTime);
        });
    }

    void EditorCameraSystem::MoveCamera(TransformComponent& transform,
        const EditorCameraComponent& editorCamera, const float deltaTime)
    {
        Vec3& position = transform.Position;
        const float displacement = editorCamera.MoveSpeed * deltaTime;
        
        if (Input::IsKeyPressed(KEY_S))
            position.z -= displacement;
        if (Input::IsKeyPressed(KEY_W))
            position.z += displacement;
            
        if (Input::IsKeyPressed(KEY_A))
            position.x -= displacement;
        if (Input::IsKeyPressed(KEY_D))
            position.x += displacement;

        if (Input::IsKeyPressed(KEY_LEFT_SHIFT))
            position.y -= displacement;
        if (Input::IsKeyPressed(KEY_SPACE))
            position.y += displacement;
    }
}