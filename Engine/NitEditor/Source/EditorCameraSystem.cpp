#include "EditorCameraSystem.h"
#include "Core/Engine.h"
#include "Component/CameraComponent.h"
#include "System/CameraSystem.h"
#include "Component/TransformComponent.h"
#include "Windows/ViewportWindow.h"
#include "Render/Renderer.h"
#include "Component/EditorCameraControllerComponent.h"

namespace Nit::EditorCameraSystem
{
    void OnCreate();
    void OnStart();
    void OnFinish();
    void OnLateUpdate();

    void Register()
    {
        Engine::CreateSystem("EditorCameraSystem", 600, ExecutionContext::Editor);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::Start, &OnStart);
        Engine::SetSystemCallback(SystemStage::LateUpdate, &OnLateUpdate);
        Engine::SetSystemCallback(SystemStage::Finish, &OnFinish);
    }

    Entity editorCameraEntity;

    void OnCreate()
    {
        EntityCreationParams creationParams;
        creationParams.Name = "Editor Camera";
        creationParams.IsSerializable = false;
        editorCameraEntity = World::CreateEntity(creationParams);
        auto& camera = editorCameraEntity.Add<CameraComponent>();
        auto& controller = editorCameraEntity.Add<EditorCameraControllerComponent>();
        CameraSystem::SetMainCameraEntity(editorCameraEntity);

        controller.DesiredZoom = camera.Size;

        Engine::GetWindow().Events().ScrollEvent.Add([&](const Vector2& offset) {

            if (!Input::IsEnabled()) return;

            controller.DesiredZoom -= offset.y * controller.ZoomStep;
            controller.DesiredZoom = std::clamp(controller.DesiredZoom, camera.NearPlane, Math::FloatMax());
        });
    }

    void OnStart()
    {
        if (!Engine::IsPaused() || !Engine::IsStopped())
        {
            CameraSystem::SetMainCameraEntity({});
        }

        auto& controller = editorCameraEntity.Get<EditorCameraControllerComponent>();
        controller.AuxCamera = editorCameraEntity.Get<CameraComponent>();
    }

    void OnLateUpdate()
    {
        if (!Engine::IsPaused() || !Engine::IsStopped())
            return;

        auto& controller = editorCameraEntity.Get<EditorCameraControllerComponent>();
        auto& camera = editorCameraEntity.Get<CameraComponent>();
        auto& transform = editorCameraEntity.Get<TransformComponent>();

        // Zoom stuff
        if (std::abs(controller.DesiredZoom - camera.Size) > .01f)
        {
            const float dir = camera.Size < controller.DesiredZoom ? 1.f : -1.f;
            camera.Size += controller.ZoomSpeed * dir * Time::GetDeltaTime();
        }
        else
        {
            camera.Size = controller.DesiredZoom;
        }

        // Move
        const bool IsRightMousePressed = Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

        // Mouse pressed
        if (!controller.MouseDown && IsRightMousePressed)
        {
            controller.MouseDown = true;
            Vector2 mouseWorld = CameraStatics::ScreenToWorldPoint(controller.AuxCamera, Input::GetMousePosition());
            controller.OffsetPos = mouseWorld + controller.AuxPosition;
        }

        // Mouse released
        if (controller.MouseDown && !IsRightMousePressed)
        {
            controller.MouseDown = false;
            controller.AuxPosition = transform.Position;
        }

        // Mouse hold
        if (IsRightMousePressed)
        {
            CameraStatics::UpdateCameraMatrices(controller.AuxCamera, controller.AuxPosition, Vector3::Zero);

            Vector2 mouseWorld = CameraStatics::ScreenToWorldPoint(controller.AuxCamera, Input::GetMousePosition());
            transform.Position = Vector3(mouseWorld * -1, transform.Position.z) + Vector3(controller.OffsetPos.x, controller.OffsetPos.y, 0);
        }

        controller.AuxCamera = camera;
    }
    
    void OnFinish()
    {
        CameraSystem::SetMainCameraEntity(editorCameraEntity);
    }
}