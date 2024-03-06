#include "EditorCameraSystem.h"
#include "Core/Engine.h"
#include "Component/CameraComponent.h"
#include "System/CameraSystem.h"
#include "Component/TransformComponent.h"
#include "Windows/ViewportWindow.h"
#include "Render/Renderer.h"

namespace Nit::EditorCameraSystem
{
    void OnCreate();
    void OnStart();
    void OnUpdate();
    void OnFinish();
    void OnLateUpdate();

    void Register()
    {
        Engine::CreateSystem("EditorCameraSystem", 600, ExecutionContext::Editor);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::Start, &OnStart);
        Engine::SetSystemCallback(SystemStage::Update, &OnUpdate);
        Engine::SetSystemCallback(SystemStage::LateUpdate, &OnLateUpdate);
        Engine::SetSystemCallback(SystemStage::Finish, &OnFinish);
    }

    Entity cameraEntity;
    float ZoomStep = 0.15f;
    float ZoomSpeed = 1.5f;
    float MoveSpeed = 2.f;
    bool CanMove = true;
    float m_DesiredZoom;
    Vector3 m_AuxPosition;
    bool m_bMouseDown = false;
    Vector3 m_Offset;
    bool bIsRightMousePressed;
    CameraComponent AuxCamera;

    CameraComponent& GetCamera()
    {
        static CameraComponent& camera = cameraEntity.Get<CameraComponent>();
        return camera;
    }

    TransformComponent& GetTransform()
    {
        static TransformComponent& transform = cameraEntity.Get<TransformComponent>();
        return transform;
    }

    void OnCreate()
    {
        EntityCreationParams creationParams;
        creationParams.Name = "Editor Camera";
        creationParams.IsSerializable = false;
        cameraEntity = World::CreateEntity(creationParams);
        cameraEntity.Add<CameraComponent>();
        CameraSystem::SetMainCameraEntity(cameraEntity);

        m_DesiredZoom = GetCamera().Size;

        Engine::GetWindow().Events().ScrollEvent.Add([&](const Vector2& offset) {

            if (!Input::IsEnabled()) return;

            m_DesiredZoom -= offset.y * ZoomStep;
            m_DesiredZoom = std::clamp(m_DesiredZoom, GetCamera().NearPlane, Math::FloatMax());
        });
    }

    void OnStart()
    {
        if (!Engine::IsPaused() || !Engine::IsStopped())
        {
            CameraSystem::SetMainCameraEntity({});
        }

        AuxCamera = GetCamera();
    }

    void OnUpdate()
    {
        
    }

    void OnLateUpdate()
    {
        if (!Engine::IsPaused() || !Engine::IsStopped())
            return;

        // Zoom stuff
        if (std::abs(m_DesiredZoom - GetCamera().Size) > .01f)
        {
            const float dir = GetCamera().Size < m_DesiredZoom ? 1.f : -1.f;
            GetCamera().Size += ZoomSpeed * dir * Time::GetDeltaTime();
        }
        else
        {
            GetCamera().Size = m_DesiredZoom;
        }

        // Move
        const bool IsRightMousePressed = Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

        // Mouse pressed
        if (!m_bMouseDown && IsRightMousePressed)
        {
            m_bMouseDown = true;
            Vector2 mouseWorld = CameraStatics::ScreenToWorldPoint(AuxCamera, Input::GetMousePosition());
            m_Offset = mouseWorld + m_AuxPosition;
        }

        // Mouse released
        if (m_bMouseDown && !IsRightMousePressed)
        {
            m_bMouseDown = false;
            m_AuxPosition = GetTransform().Position;
        }

        // Mouse hold
        if (IsRightMousePressed)
        {
            CameraStatics::UpdateCameraMatrices(AuxCamera, m_AuxPosition, Vector3::Zero);

            Vector2 mouseWorld = CameraStatics::ScreenToWorldPoint(AuxCamera, Input::GetMousePosition());
            GetTransform().Position = Vector3(mouseWorld * -1, GetTransform().Position.z) + Vector3(m_Offset.x, m_Offset.y, 0);
        }

        AuxCamera = GetCamera();
    }
    
    void OnFinish()
    {
        if (Engine::IsPaused() || Engine::IsStopped())
        {
            CameraSystem::SetMainCameraEntity(cameraEntity);
        }
    }
}