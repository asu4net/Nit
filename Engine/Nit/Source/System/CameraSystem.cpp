#include "CameraSystem.h"
#include "Component\CameraComponent.h"
#include "Component\TransformComponent.h"
#include "Render\Renderer.h"
#include "Core\Engine.h"

namespace Nit::CameraSystem
{
    Entity MainCameraEntity;
    const TString SystemID = "CameraSystem";

    void Register()
    {
        Engine::CreateSystem(SystemID, CameraExecutionOrder, ExecutionContext::Always);
        Engine::SetSystemCallback(SystemStage::Start, &OnStart);
        Engine::SetSystemCallback(SystemStage::Update, &OnUpdate);
    }

    void SetMainCameraEntity(Entity entity)
    {
        MainCameraEntity = entity;
    }

    Entity GetMainCameraEntity()
    {
        return MainCameraEntity;
    }

    void OnStart()
    {
        const auto view = World::GetRegistry().view<TransformComponent, CameraComponent>();

        view.each([&](RawEntity entity, TransformComponent&, CameraComponent& cameraComponent) {
            if (cameraComponent.IsStartCamera)
                MainCameraEntity = entity;
        });
    }

    void OnUpdate()
    {
        const auto view = World::GetRegistry().view<TransformComponent, CameraComponent>();

        view.each([&](RawEntity entity, TransformComponent&, CameraComponent& cameraComponent) {
            CameraStatics::UpdateCameraMatrices({entity});
        });
        
        Renderer::SetErrorScreenEnabled(!MainCameraEntity.IsValid());

        if (!MainCameraEntity.IsValid() || (MainCameraEntity.IsValid() && !MainCameraEntity.Has<CameraComponent>()))
        {
            return;
        }

        const CameraComponent& mainCamera = MainCameraEntity.Get<CameraComponent>();
        Renderer::SetProjectionViewMatrix(mainCamera.ProjectionMatrix * mainCamera.ViewMatrix);
    }
}