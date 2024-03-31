#include "GizmoSystem.h"
#include "Component/BoxCollider2DComponent.h"
#include "Component/CircleColliderComponent.h"
#include "Component/Rigidbody2DComponent.h"
#include "Core/Engine.h"
#include "Entity/Entity.h"

namespace Nit::GizmoSystem
{
    //cuando se añaden colliders se crean primitivas por cada entidad
    void OnCreate();
    void OnComponentAdded(Registry&, RawEntity rawEntity);
    void OnComponentRemoved(Registry&, RawEntity rawEntity);
    void OnPreDrawPrimitives();
    
    void Register()
    {
        Engine::CreateSystem("GizmoSystem", SpriteExecutionOrder + 1000, ExecutionContext::Always);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::PreDrawPrimitives, &OnPreDrawPrimitives);
    }

    void OnCreate()
    {
        Registry& registry = World::GetRegistry();
        registry.on_construct<BoxCollider2DComponent>  ().connect<&OnComponentAdded>();
        registry.on_construct<CircleColliderComponent> ().connect<&OnComponentAdded>();
        registry.on_construct<Rigidbody2DComponent>    ().connect<&OnComponentAdded>();
        registry.on_destroy  <Rigidbody2DComponent>    ().connect<&OnComponentRemoved>();
    }

    void OnComponentAdded(Registry&, RawEntity rawEntity)
    {
        Entity entity = rawEntity;
        
        // if (!entity.Has<Rigidbody2DComponent>()) return;
        // auto& pb = entity.Get<Rigidbody2DComponent>();
        // if (pb.BodyPtr) return;
        // const auto& transform = entity.Get<TransformComponent>();
        //
        // if (entity.Has<BoxCollider2DComponent>())
        // {
        //     auto& boxCollider = entity.Get<BoxCollider2DComponent>();
        //     if (boxCollider.FixturePtr) return;
        //     // create primitive
        // }
        //
        // else if (entity.Has<CircleColliderComponent>())
        // {
        //     auto& circleCollider = entity.Get<CircleColliderComponent>();
        //     if (circleCollider.FixturePtr) return;
        //     // create primitive
        // }
    }

    void OnComponentRemoved(Registry&, RawEntity rawEntity)
    {
        // Entity entity = rawEntity;
        //
        // if (!entity.Has<Rigidbody2DComponent>()) return;
        //
        // auto& pb = entity.Get<Rigidbody2DComponent>();
        // destroy primitive
    }

    void OnPreDrawPrimitives()
    {
        //Registry& registry = World::GetRegistry();
        
        // Update CircleColliders
        // {
        //     registry.view<CircleColliderComponent, TransformComponent, Rigidbody2DComponent>()
        //    .each([&](const auto entity, CircleColliderComponent& collider, TransformComponent& transform, Rigidbody2DComponent& physicBody)
        //    {
        //        
        //    });
        // }
        //
        // // Update BoxColliders
        // {
        //     registry.view<BoxCollider2DComponent, TransformComponent, Rigidbody2DComponent>()
        //     .each([&](const auto entity, BoxCollider2DComponent& collider, TransformComponent& transform, Rigidbody2DComponent& physicBody)
        //     {
        //         
        //     });
        // }
    }

}
