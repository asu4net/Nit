#include "CircleSystem.h"

#include "FamilySystem.h"
#include "Core\Engine.h"
#include "Component\CircleComponent.h"
#include "Render\Renderer.h"
#include "Component\TransformComponent.h"

namespace Nit::CircleSystem
{
    void OnCreate();
    void OnCircleComponentAdded(Registry&, RawEntity entity);
    void OnCircleComponentRemoved(Registry&, RawEntity entity);
    void OnPreDrawPrimitives();

    void Register()
    {
        Engine::CreateSystem("CircleSystem", CircleExecutionOrder, ExecutionContext::Always);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::PreDrawPrimitives, &OnPreDrawPrimitives);
    }

    void OnCreate()
    {
        World::GetRegistry().on_construct<CircleComponent>().connect<&OnCircleComponentAdded>();
        World::GetRegistry().on_destroy<CircleComponent>().connect<&OnCircleComponentRemoved>();
    }

    void OnCircleComponentAdded(Registry&, RawEntity entity)
    {
        Entity circleEntity = entity;
        CircleComponent& circleComponent = circleEntity.Get<CircleComponent>();
        circleComponent.Primitive = Renderer::CreatePrimitive<CirclePrimitive>();
    }

    void OnCircleComponentRemoved(Registry&, RawEntity entity)
    {
        Entity circleEntity = entity;
        CircleComponent& circleComponent = circleEntity.Get<CircleComponent>();
        Renderer::DestroyPrimitive(circleComponent.Primitive);
        circleComponent.Primitive = nullptr;
    }

    void OnPreDrawPrimitives()
    {
        const auto view = World::GetRegistry().view<TransformComponent, CircleComponent>();

        view.each([&](RawEntity rawEntity, const TransformComponent& transformComponent, const CircleComponent& circle) {

            auto& primitive = *circle.Primitive;
            primitive.bIsVisible = circle.IsVisible;

            if (!circle.IsVisible)
                return;

            Entity entity = rawEntity;

            primitive.Transform = FamilySystem::GetMatrix(entity);
            primitive.TintColor = circle.TintColor;
            primitive.Radius = circle.Radius;
            primitive.Thickness = circle.Thickness;
            primitive.Fade = circle.Fade;

            primitive.EntityID = (int) rawEntity;
            primitive.SortingLayer = circle.SortingLayer;
        });
    }
}