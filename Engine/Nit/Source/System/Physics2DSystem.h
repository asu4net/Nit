#pragma once

namespace Nit
{
    struct Rigidbody2DComponent;
    struct BoxCollider2DComponent;
    struct CircleColliderComponent;
}

namespace Nit::Physics2DSystem
{
    void    SetGravity(const Vector2& gravity);
    Vector2 GetGravity();

    void    SetVelocityIterations(float velocityIterations);
    float   GetVelocityIterations();

    void    SetPositionIterations(float positionIterations);
    float   GetPositionIterations();
    
    void    ApplyForce(Rigidbody2DComponent& body, const Vector2& force, const Vector2& position, bool impulse);
    void    ApplyTorque(Rigidbody2DComponent& body, float torque, bool impulse);
    
    void Register();    
}