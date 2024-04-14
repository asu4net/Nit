#pragma once

namespace Nit
{
    struct Rigidbody2DComponent;
    struct BoxCollider2DComponent;
    struct CircleColliderComponent;
}

namespace Nit::Physics2DSystem
{
    void    SetGravity(const CVector2& gravity);
    CVector2 GetGravity();

    void    SetVelocityIterations(float velocityIterations);
    float   GetVelocityIterations();

    void    SetPositionIterations(float positionIterations);
    float   GetPositionIterations();
    
    void    ApplyForce(Rigidbody2DComponent& body, const CVector2& force, const CVector2& position, bool impulse);
    void    ApplyTorque(Rigidbody2DComponent& body, float torque, bool impulse);
    
    void Register();    
}