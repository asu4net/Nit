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
        collider.PrevSize = collider.Size;
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
        collider.PrevRadius = collider.Radius;
    }

    void UpdateBodyData(Rigidbody2DComponent& rb, TransformComponent& transform, const Vector2& center)
    {
        b2Body* body = static_cast<b2Body*>(rb.BodyPtr);

        if (!rb.IsEnabled)
        {
            body->SetEnabled(false);
            return;
        }
                
        body->SetEnabled(true);

        if (rb.FollowTransform)
        {
            body->SetAwake(false);
            body->SetTransform(ToBox2D(transform.Position), Math::ToRadians(transform.Rotation.z));
        }
        else
        {
            body->SetAwake(true);
            transform.Position   = Vector3(FromBox2D(body->GetTransform().p - ToBox2D(center)), transform.Position.z);
            transform.Rotation.z = Math::ToDegrees(body->GetAngle());
        }
        
        body->SetGravityScale(rb.GravityScale);
    }

    void TryCreateBox2DBodies()
    {
        auto view = World::GetRegistry().view<TransformComponent, Rigidbody2DComponent>();
        
        for (RawEntity rawEntity : view)
        {
            auto [transform, rb] = view.get<TransformComponent, Rigidbody2DComponent>(rawEntity);
            Entity entity = rawEntity;

            // Check if re-creation needed
            {
                if (entity.Has<BoxCollider2DComponent>())
                {
                    auto& boxCollider = entity.Get<BoxCollider2DComponent>();

                    if (!rb.BodyPtr)
                    {
                        CreateBody(rb, transform.Position + boxCollider.Center, transform.Rotation.z);
                    }

                    if (!boxCollider.FixturePtr || boxCollider.Size != boxCollider.PrevSize)
                    {
                        CreateBoxFixture(rb, boxCollider);
                    }
                }
                else if (entity.Has<CircleColliderComponent>())
                {
                    auto& circleCollider = entity.Get<CircleColliderComponent>();

                    if (!rb.BodyPtr)
                    {
                        CreateBody(rb, transform.Position + circleCollider.Center, transform.Rotation.z);
                    }

                    if (circleCollider.FixturePtr || circleCollider.Radius != circleCollider.PrevRadius)
                    {
                        CreateCircleFixture(rb, circleCollider);
                    }
                }
            }

            // Check for changes on body type
            {
                b2Body* body = static_cast<b2Body*>(rb.BodyPtr);
                if (rb.BodyType != rb.PrevBodyType)
                {
                    body->SetType(static_cast<b2BodyType>(rb.BodyType));
                    if (rb.BodyType == EBodyType::Kinematic)
                        body->SetLinearVelocity({ 0, 0 });
                }
            }
        }
    }
    
    void OnCreate();
    void OnBoxCollider2DAdded(Registry&, RawEntity rawEntity);
    void OnCircleColliderAdded(Registry&, RawEntity rawEntity);
    void OnRigidbody2DRemoved(Registry&, RawEntity rawEntity);
    void OnDestroy();
    void OnStart();
    void OnUpdate();
    void OnFixedUpdate();

    void Register()
    {
        Engine::CreateSystem("Physics2DSystem", PhysicsExecutionOrder, ExecutionContext::Runtime);
        Engine::SetSystemCallback(SystemStage::Create,      &OnCreate);
        Engine::SetSystemCallback(SystemStage::Destroy,     &OnDestroy);
        Engine::SetSystemCallback(SystemStage::Start,       &OnStart);
        Engine::SetSystemCallback(SystemStage::Update,      &OnUpdate);
        Engine::SetSystemCallback(SystemStage::FixedUpdate, &OnFixedUpdate);
    }

    void OnCreate()
    {
        PhysicWorld = new b2World({Gravity.x, Gravity.y});
        
        Registry& registry = World::GetRegistry();
        registry.on_construct<BoxCollider2DComponent>  ().connect<&OnBoxCollider2DAdded>();
        registry.on_construct<CircleColliderComponent> ().connect<&OnBoxCollider2DAdded>();
        registry.on_destroy  <Rigidbody2DComponent>    ().connect<&OnRigidbody2DRemoved>();
    }

    void OnDestroy()
    {
        Registry& registry = World::GetRegistry();
        registry.on_construct<BoxCollider2DComponent>  ().disconnect<&OnBoxCollider2DAdded>();
        registry.on_construct<CircleColliderComponent> ().disconnect<&OnCircleColliderAdded>();
        registry.on_destroy  <Rigidbody2DComponent>    ().disconnect<&OnRigidbody2DRemoved>();
    }

    void OnBoxCollider2DAdded(Registry&, RawEntity rawEntity)
    {
        Entity entity = rawEntity;
        if (entity.Has<CircleColliderComponent>())
        {
            entity.Remove<BoxCollider2DComponent>();
        }
    }

    void OnCircleColliderAdded(Registry&, RawEntity rawEntity)
    {
        Entity entity = rawEntity;
        if (entity.Has<BoxCollider2DComponent>())
        {
            entity.Remove<CircleColliderComponent>();
        }
    }

    void OnRigidbody2DRemoved(Registry&, RawEntity rawEntity)
    {
        Entity entity = rawEntity;
        auto& rb = entity.Get<Rigidbody2DComponent>();
        PhysicWorld->DestroyBody(static_cast<b2Body*>(rb.BodyPtr));
        if (entity.Has<BoxCollider2DComponent>())
        {
            entity.Get<BoxCollider2DComponent>().FixturePtr = nullptr;
        }
        else if (entity.Has<CircleColliderComponent>())
        {
            entity.Get<CircleColliderComponent>().FixturePtr = nullptr;
        }
    }

    void OnStart()
    {
        TryCreateBox2DBodies();
        // Set the start positions
        {
            World::GetRegistry().view<TransformComponent, Rigidbody2DComponent>()
             .each([&](const auto entity, TransformComponent& transform, Rigidbody2DComponent& rb)
             {
                 b2Body* body   = static_cast<b2Body*>(rb.BodyPtr);
                 body->SetTransform(ToBox2D(transform.Position), Math::ToRadians(transform.Rotation.z));  
             });
        }
    }

    void OnUpdate()
    {
        TryCreateBox2DBodies();
    }
    
    void OnFixedUpdate()
    {
        // Update physic world
        PhysicWorld->SetGravity(ToBox2D(Gravity));
        PhysicWorld->Step(Time::FixedDeltaTime, VelocityIterations, PositionIterations);

        auto view = World::GetRegistry().view<TransformComponent, Rigidbody2DComponent>();
        
        for (RawEntity rawEntity : view)
        {
            auto [transform, rb] = view.get<TransformComponent, Rigidbody2DComponent>(rawEntity);
            Entity entity = rawEntity;
            
            if (entity.Has<BoxCollider2DComponent>())
            {
                auto& boxCollider = entity.Get<BoxCollider2DComponent>();
                UpdateBodyData(rb, transform, boxCollider.Center);

            }
            else if (entity.Has<CircleColliderComponent>())
            {
                auto& circleCollider = entity.Get<CircleColliderComponent>();
                UpdateBodyData(rb, transform, circleCollider.Center);
            }
        }
    }
}