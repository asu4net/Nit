#include "Physics2DSystem.h"
#include "Asset/PhysicMaterial.h"
#include "box2d/b2_body.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_world.h"
#include "Component/BoxCollider2DComponent.h"
#include "Component/CircleColliderComponent.h"
#include "Component/Rigidbody2DComponent.h"
#include "Component/TransformComponent.h"
#include "Core/Engine.h"

namespace Nit::Physics2DSystem
{
    b2World* PhysicWorld        = nullptr;
    Vector2  Gravity            = { 0.f, -9.8f };
    uint32_t VelocityIterations = 6;
    uint32_t PositionIterations = 2;

    b2Vec2 ToBox2D(const Vector2& vec)
    {
        return b2Vec2(vec.x, vec.y);
    }

    Vector2 FromBox2D(const b2Vec2& vec)
    {
        return Vector2(vec.x, vec.y);
    }

    void SetGravity(const Vector2& gravity)
    {
        Gravity = gravity;
    }

    Vector2 GetGravity()
    {
        return Gravity;
    }

    void SetVelocityIterations(float velocityIterations)
    {
        VelocityIterations = velocityIterations;
    }

    float GetVelocityIterations()
    {
        return VelocityIterations;
    }

    void SetPositionIterations(float positionIterations)
    {
        PositionIterations = positionIterations;
    }

    float GetPositionIterations()
    {
        return PositionIterations;
    }

    void ApplyForce(Rigidbody2DComponent& body, const Vector2& force, const Vector2& position, bool impulse)
    {
        b2Body* b2Body = static_cast<::b2Body*>(body.BodyPtr);
        b2Body->ApplyForce({force.x, force.y}, {position.x, position.y}, impulse);
    }

    void ApplyTorque(Rigidbody2DComponent& body, float torque, bool impulse)
    {
        b2Body* b2Body = static_cast<::b2Body*>(body.BodyPtr);
        b2Body->ApplyTorque(torque, impulse);
    }
    
    void CreateBody(Rigidbody2DComponent& rb, const Vector2& position, float angle)
    {
        b2BodyDef bodyDef;
        bodyDef.type      = static_cast<b2BodyType>(rb.BodyType);
        bodyDef.position  = ToBox2D(position);
        bodyDef.angle     = angle;
        rb.PrevBodyType   = rb.BodyType;
        rb.BodyPtr        = PhysicWorld->CreateBody(&bodyDef);
    }

    void SetFixtureDefinition(b2FixtureDef& fixtureDef, const b2Shape& shape, AssetRef materialRef)
    {
        static PhysicMaterial defaultMaterial;
        const PhysicMaterial& material = materialRef.Is<PhysicMaterial>() ?
            materialRef.As<PhysicMaterial>() : defaultMaterial;
        
        fixtureDef.shape                = &shape;
        fixtureDef.density              = material.Density;
        fixtureDef.friction             = material.Friction;
        fixtureDef.restitution          = material.Bounciness;
        fixtureDef.restitutionThreshold = material.BouncinessThreshold;
    }
    
    void CreateBoxFixture(Rigidbody2DComponent& rb, BoxCollider2DComponent& collider)
    {
        b2Fixture* fixture = static_cast<b2Fixture*>(collider.FixturePtr);
        b2Body* body = static_cast<b2Body*>(rb.BodyPtr);
        
        if (fixture)
            body->DestroyFixture(fixture);

        b2PolygonShape boxShape;
        boxShape.SetAsBox(collider.Size.x / 2, collider.Size.y / 2);
        b2FixtureDef fixtureDef;
        SetFixtureDefinition(fixtureDef, boxShape, collider.PhysicMaterialRef);
        collider.FixturePtr = static_cast<void*>(body->CreateFixture(&fixtureDef));
    }

    void CreateCircleFixture(Rigidbody2DComponent& pb2D, CircleColliderComponent& collider)
    {
        b2Fixture* fixture = static_cast<b2Fixture*>(collider.FixturePtr);
        b2Body* body = static_cast<b2Body*>(pb2D.BodyPtr);
        
        if (fixture)
            body->DestroyFixture(fixture);

        b2CircleShape circleShape;
        circleShape.m_radius = collider.Radius;
        b2FixtureDef fixtureDef;
        SetFixtureDefinition(fixtureDef, circleShape, collider.PhysicMaterialRef);
        collider.FixturePtr = static_cast<void*>(body->CreateFixture(&fixtureDef));
    }

    void UpdateBodyData(Rigidbody2DComponent& rb)
    {
        b2Body* body = static_cast<b2Body*>(rb.BodyPtr);
        body->SetGravityScale(rb.GravityScale);
        
        if (rb.BodyType != rb.PrevBodyType)
        {
            body->SetType(static_cast<b2BodyType>(rb.BodyType));
            if (rb.BodyType == EBodyType::Kinematic)
                body->SetLinearVelocity({ 0, 0 });
        }
    }
    
    void OnCreate();
    void OnComponentAdded(Registry&, RawEntity rawEntity);
    void OnComponentRemoved(Registry&, RawEntity rawEntity);
    void OnDestroy();
    void OnStart();
    void OnFixedUpdate();

    void Register()
    {
        Engine::CreateSystem("Physics2DSystem", PhysicsExecutionOrder, ExecutionContext::Runtime);
        Engine::SetSystemCallback(SystemStage::Create,      &OnCreate);
        Engine::SetSystemCallback(SystemStage::Destroy,     &OnDestroy);
        Engine::SetSystemCallback(SystemStage::Start,       &OnStart);
        Engine::SetSystemCallback(SystemStage::FixedUpdate, &OnFixedUpdate);
    }

    void OnCreate()
    {
        PhysicWorld = new b2World({Gravity.x, Gravity.y});
        
        Registry& registry = World::GetRegistry();
        registry.on_construct<BoxCollider2DComponent>  ().connect<&OnComponentAdded>();
        registry.on_construct<CircleColliderComponent> ().connect<&OnComponentAdded>();
        registry.on_construct<Rigidbody2DComponent>    ().connect<&OnComponentAdded>();
        registry.on_destroy  <Rigidbody2DComponent>    ().connect<&OnComponentRemoved>();
    }

    void OnDestroy()
    {
        Registry& registry = World::GetRegistry();
        registry.on_construct<BoxCollider2DComponent>  ().disconnect<&OnComponentAdded>();
        registry.on_construct<CircleColliderComponent> ().disconnect<&OnComponentAdded>();
        registry.on_construct<Rigidbody2DComponent>    ().disconnect<&OnComponentAdded>();
        registry.on_destroy  <Rigidbody2DComponent>    ().disconnect<&OnComponentRemoved>();
    }

    void OnComponentAdded(Registry&, RawEntity rawEntity)
    {
        Entity entity = rawEntity;
        
        if (!entity.Has<Rigidbody2DComponent>()) return;
        auto& pb = entity.Get<Rigidbody2DComponent>();
        
        if (pb.BodyPtr) return;
        const auto& transform = entity.Get<TransformComponent>();

        if (entity.Has<BoxCollider2DComponent>())
        {
            auto& boxCollider = entity.Get<BoxCollider2DComponent>();
            if (boxCollider.FixturePtr) return;
            boxCollider.PrevSize = boxCollider.Size;
            CreateBody(pb, transform.Position + boxCollider.Center, transform.Rotation.z);
            CreateBoxFixture(pb, boxCollider);
        }
        
        else if (entity.Has<CircleColliderComponent>())
        {
            auto& circleCollider = entity.Get<CircleColliderComponent>();
            if (circleCollider.FixturePtr) return;
            circleCollider.Radius = circleCollider.Radius;
            CreateBody(pb, transform.Position + circleCollider.Center, transform.Rotation.z);
            CreateCircleFixture(pb, circleCollider);
        }
    }

    void OnComponentRemoved(Registry&, RawEntity rawEntity)
    {
        Entity entity = rawEntity;

        if (!entity.Has<Rigidbody2DComponent>()) return;
        
        auto& pb = entity.Get<Rigidbody2DComponent>();
        PhysicWorld->DestroyBody(static_cast<b2Body*>(pb.BodyPtr));
        pb.BodyPtr = nullptr;
        
        if (entity.Has<BoxCollider2DComponent>())
            entity.Get<BoxCollider2DComponent>().FixturePtr = nullptr;

        if (entity.Has<CircleColliderComponent>())
            entity.Get<CircleColliderComponent>().FixturePtr = nullptr;
    }

    void OnStart()
    {
        Registry& registry = World::GetRegistry();

        // Init circle colliders
        {
            registry.view<CircleColliderComponent, TransformComponent, Rigidbody2DComponent>()
             .each([&](const auto entity, CircleColliderComponent& collider, TransformComponent& transform, Rigidbody2DComponent& rb)
             {
                 b2Body* body   = static_cast<b2Body*>(rb.BodyPtr);
                 body->SetTransform(ToBox2D(transform.Position), transform.Rotation.z);  
             });
        }

        // Init box colliders
        {
            registry.view<BoxCollider2DComponent, TransformComponent, Rigidbody2DComponent>()
               .each([&](const auto entity, BoxCollider2DComponent& collider, TransformComponent& transform, Rigidbody2DComponent& rb)
               {
                  b2Body* body   = static_cast<b2Body*>(rb.BodyPtr);
                  body->SetTransform(ToBox2D(transform.Position), transform.Rotation.z);  
               });
        }
    }

    void OnFixedUpdate()
    {
        // Update physic world
        PhysicWorld->SetGravity(ToBox2D(Gravity));
        PhysicWorld->Step(Time::FixedDeltaTime, VelocityIterations, PositionIterations);
        
        Registry& registry = World::GetRegistry();
        
        // Update CircleColliders
        {
            registry.view<CircleColliderComponent, TransformComponent, Rigidbody2DComponent>()
           .each([&](const auto entity, CircleColliderComponent& collider, TransformComponent& transform, Rigidbody2DComponent& physicBody)
           {
               if (collider.Radius != collider.PrevRadius)
                   CreateCircleFixture(physicBody, collider);
            
               const b2Body* body   = static_cast<b2Body*>(physicBody.BodyPtr);
               transform.Position   = Vector3(FromBox2D(body->GetTransform().p - ToBox2D(collider.Center)), transform.Position.z);
               transform.Rotation.z = Math::ToDegrees(body->GetAngle());
               UpdateBodyData(physicBody);
           });
        }

        // Update BoxColliders
        {
            registry.view<BoxCollider2DComponent, TransformComponent, Rigidbody2DComponent>()
            .each([&](const auto entity, BoxCollider2DComponent& collider, TransformComponent& transform, Rigidbody2DComponent& physicBody)
            {
                if (collider.Size != collider.PrevSize)
                    CreateBoxFixture(physicBody, collider);

               const b2Body* body   = static_cast<b2Body*>(physicBody.BodyPtr);
               transform.Position   = Vector3(FromBox2D(body->GetTransform().p - ToBox2D(collider.Center)), transform.Position.z);
               transform.Rotation.z = Math::ToDegrees(body->GetAngle());
               UpdateBodyData(physicBody);
            });
        }
    }

}
