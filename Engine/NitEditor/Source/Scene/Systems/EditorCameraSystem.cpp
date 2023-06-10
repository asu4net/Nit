#include "EditorCameraSystem.h"
#include "Scene/Components/EditorCameraComponent.h"

namespace Nit { NIT_FORCE_LINK_IMPL(EditorCameraSystem) }

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
        Engine::GetInstance().GetWindow()->Events().ScrollEvent.Add([&](const Vec2& offset)
        {
            if (Input::IsConsumedByEditor() || !m_EditorCameraActor.IsValid())
                return;

            auto& editorCamera = m_EditorCameraActor.Get<EditorCameraComponent>();

            if (editorCamera.CameraData.Projection != CameraProjection::Orthographic)
                return;

            const bool bShiftPressed = Input::IsKeyPressed(KEY_LEFT_SHIFT);
            
            editorCamera.CameraData.Size -= offset.y * editorCamera.ZoomStep *
                (bShiftPressed ? 2 : 1);
            editorCamera.CameraData.Size = glm::clamp(editorCamera.CameraData.Size,
                editorCamera.CameraData.NearPlane,  std::numeric_limits<float>::max());
        });
    }

    void EditorCameraSystem::OnUpdate(const TimeStep& timeStep)
    {
        if (GetScene().IsRuntimeEnabled()) return;
        
        const auto view = Registry().view<TransformComponent, EditorCameraComponent>();
        
        m_EditorCameraActor = { view.front(), RegistryPtr() };
        
        view.each([&](TransformComponent& transform, EditorCameraComponent& camera)
        {
            camera.CameraData.ScreenWidth = Engine::GetInstance().GetScreenWidth();
            camera.CameraData.ScreenHeight = Engine::GetInstance().GetScreenHeight();

            m_AuxCamera = camera.CameraData;
            
            camera.CameraData.CalculateProjectionViewMat4(transform.Position, transform.Rotation);
            GetScene().GetSceneRenderer().PushCamera(camera.CameraData);

            if (camera.CameraData.Projection == CameraProjection::Orthographic)
                MoveOrthographicCamera(transform, camera, timeStep.DeltaTime);
            else if (camera.CameraData.Projection == CameraProjection::Perspective)
                MovePerspectiveCamera(transform, camera, timeStep.DeltaTime);
        });
    }

    void EditorCameraSystem::MoveOrthographicCamera(TransformComponent& transform,
        EditorCameraComponent& editorCamera, const float deltaTime)
    {
        if (!editorCamera.bCanMove)
            return;

        const bool bRightMousePressed = Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT); 
        
        // Mouse pressed
        if (!m_bMouseDown && bRightMousePressed)
        {
            m_bMouseDown = true;
            m_Offset = CameraStatics::ScreenToWorldPoint(editorCamera.CameraData,
                Input::GetMousePosition()) + m_AuxPosition;
        }

        // Mouse released
        if (m_bMouseDown && !bRightMousePressed)
        {
            m_bMouseDown = false;
            m_AuxPosition = transform.Position;
        }

        // Mouse hold
        if (bRightMousePressed)
        {
            m_AuxCamera.CalculateProjectionViewMat4(m_AuxPosition, VecZero);
        
            const Vec2 mouseWorld = CameraStatics::ScreenToWorldPoint(m_AuxCamera,
                Input::GetMousePosition());
        
            transform.Position = Vec3(-mouseWorld, transform.Position.z) + Vec3(m_Offset.x, m_Offset.y, 0);
        }
    }

    void EditorCameraSystem::MovePerspectiveCamera(TransformComponent& transform,
        EditorCameraComponent& editorCamera, float deltaTime)
    {
        if (!editorCamera.bCanMove)
            return;
        
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
