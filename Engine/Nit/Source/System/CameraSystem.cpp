#include "CameraSystem.h"
#include "Component\CameraComponent.h"
#include "Component\TransformComponent.h"
#include "Render\Renderer.h"
#include "Core\Engine.h"

namespace Nit::CameraSystem
{
    Entity MainCameraEntity;
    const String SystemID = "CameraSystem";

    void Register()
    {
        Engine::CreateSystem(SystemID, CameraExecutionOrder, ExecutionContext::Always);
        Engine::SetSystemCallback(SystemStage::Update, &OnUpdate);
    }

    void SetMainCameraEntity(Entity entity)
    {
        NIT_CHECK(entity.Has<CameraComponent>(), "Main camera actor should use have a CameraComponent");
        MainCameraEntity = entity;
    }

    Entity GetMainCameraEntity()
    {
        return MainCameraEntity;
    }

    void OnUpdate()
    {
        const auto view = World::GetRegistry().view<TransformComponent, CameraComponent>();
                
        view.each([&](RawEntity entity, TransformComponent&, CameraComponent& cameraComponent) {
            CameraStatics::UpdateCameraMatrices({entity});
        });
        
        NIT_CHECK(MainCameraEntity.IsValid(), "MainCameraActor entity not assigned!");
        MainCameraEntity = view.front();
        const CameraComponent& mainCamera = MainCameraEntity.Get<CameraComponent>();
        Renderer::SetProjectionViewMatrix(mainCamera.ProjectionMatrix * mainCamera.ViewMatrix);
    }
}