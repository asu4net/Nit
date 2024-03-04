#pragma once
#include "Nit.h"
#include "System/CameraSystem.h"

namespace Nit
{
    class MoveScript : public Script
    {
        float speed = 100.f;
        Vector3 jaja;

        void OnUpdate()
        {
            if (Input::IsKeyPressed(KeyCode::D))
            {
                GetTransform().Rotation += Vector3::Forward * speed * Time::GetDeltaTime();
            }
            if (Input::IsKeyPressed(KeyCode::A))
            {
                GetTransform().Rotation += Vector3::Back * speed * Time::GetDeltaTime();
            }
        }

        RTTR_ENABLE(Script)
    };

    class ColorScript : public Script
    {
        void OnSpacePressed(const InputActionContext& context)
        {
            if (context.IsPressed)
            {
                static bool bSwitch = false;
                Get<SpriteComponent>().TintColor = bSwitch ? Color::Blue : Color::Yellow;
                bSwitch = !bSwitch;
            }
        }

        void OnStart()
        {
            m_InputAction = Input::RegisterInputAction(KeyCode::Space);
            m_InputAction.lock()->OnPerformed().AddRaw(this, &ColorScript::OnSpacePressed);
        }

        void OnFinish()
        {
            Input::UnregisterInputAction(m_InputAction);
        }

        void OnSubmitGizmos()
        {
            SpritePrimitive lineSprite;
            lineSprite.TintColor = Color::Yellow;
            lineSprite.GenerateVertexData(SpriteShape::Line);
            Renderer::SubmitSpritePrimitive(lineSprite);

            SpritePrimitive circleSprite;
            circleSprite.TintColor = Color::Green;
            circleSprite.GenerateVertexData(SpriteShape::Circle);
            Renderer::SubmitSpritePrimitive(circleSprite);

            SpritePrimitive rectSprite;
            rectSprite.TransformMatrix = Matrix4::CreateTransform(Vector3::Down + Vector3::Right);
            rectSprite.TintColor = Color::Clear;
            rectSprite.GenerateVertexData(SpriteShape::Rect);
            Renderer::SubmitSpritePrimitive(rectSprite);
        }

        WeakPtr<InputAction> m_InputAction;

        RTTR_ENABLE(Script)
    };
}

RTTR_REGISTRATION
{
    rttr::registration::class_<Nit::MoveScript>("MoveScript").constructor<>();
    rttr::registration::class_<Nit::ColorScript>("ColorScript").constructor<>();
}