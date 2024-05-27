#include "InputAction.h"
#include "Core\Engine.h"
#include "InputModifiers.h"

namespace Nit
{
    InputAction::InputAction(String keyCode)
        : m_KeyCode(keyCode)
    {}

    InputAction::~InputAction() 
    {
        m_InputPerformedEvent.RemoveAll();
        m_InputModifiers.clear();
    };

    void InputAction::Update()
    {
        /*
        if (!m_IsKeyPressed && Input::IsKeyPressed(m_KeyCode))
        {
            m_IsKeyPressed = true;
            InputActionContext context;
            context.IsPressed = m_IsKeyPressed;
            m_InputPerformedEvent.Broadcast(context);
        }
        else if (m_IsKeyPressed && !Input::IsKeyPressed(m_KeyCode))
        {
            m_IsKeyPressed = false;
            InputActionContext context;
            context.IsReleased = !m_IsKeyPressed;
            m_InputPerformedEvent.Broadcast(context);
        }
        */
    }


    void InputAction::OnControllerAnalog(int _controllerId, float _analogValue)
    {
        InputActionContext context;
        context.inputType = EInputType::Axis1D;
        context.inputValue = Vector4(_analogValue, 0.f, 0.f, 0.f);

        for (const InputModifier* inputModifier : m_InputModifiers)
        {
            inputModifier->Modify(context.inputValue, context.inputType);
        }

        context.controllerId = _controllerId;
        m_InputPerformedEvent.Broadcast(context);
    }

    void InputAction::OnControllerVector2(int _controllerId, const Vector2& _vector2Value)
    {
        InputActionContext context;
        context.inputType = EInputType::Axis2D;
        context.inputValue = Vector4(_vector2Value.x, _vector2Value.y, 0.f, 0.f);

        for (const InputModifier* inputModifier : m_InputModifiers)
        {
            inputModifier->Modify(context.inputValue, context.inputType);
        }

        context.controllerId = _controllerId;
        m_InputPerformedEvent.Broadcast(context);
    }

    void InputAction::OnControllerVector3(int _controllerId, const Vector3& _vector3Value)
    {
        InputActionContext context;
        context.inputType = EInputType::Axis3D;
        context.inputValue = Vector4(_vector3Value.x, _vector3Value.y, _vector3Value.z, 0.f);

        for (const InputModifier* inputModifier : m_InputModifiers)
        {
            inputModifier->Modify(context.inputValue, context.inputType);
        }

        context.controllerId = _controllerId;
        m_InputPerformedEvent.Broadcast(context);
    }

    void InputAction::OnControllerVector4(int _controllerId, const Vector4& _vector4Value)
    {
        InputActionContext context;
        context.inputType = EInputType::Axis4D;
        context.inputValue = Vector4(_vector4Value.x, _vector4Value.y, _vector4Value.z, _vector4Value.w);

        for (const InputModifier* inputModifier : m_InputModifiers)
        {
            inputModifier->Modify(context.inputValue, context.inputType);
        }

        context.controllerId = _controllerId;
        m_InputPerformedEvent.Broadcast(context);
    }

    void InputAction::OnControllerButtonPressed(int _controllerId, bool _bIsRepeat)
    {
        if((m_TriggerType == ETriggerType::Pressed && _bIsRepeat) || m_TriggerType == ETriggerType::Released) return;

        InputActionContext context;
        context.inputType = EInputType::Digital;
        context.inputValue = Vector4(1.f, 0.f, 0.f, 0.f);

        for (const InputModifier* inputModifier : m_InputModifiers)
        {
            inputModifier->Modify(context.inputValue, context.inputType);
        }

        context.bIsPressed = true;
        context.bIsRepeat = _bIsRepeat;
        context.controllerId = _controllerId;
        m_InputPerformedEvent.Broadcast(context);
    }

    void InputAction::OnControllerButtonReleased(int _controllerId, bool _bIsRepeat)
    {
        if (m_TriggerType == ETriggerType::Pressed || m_TriggerType == ETriggerType::Down) return;

        InputActionContext context;
        context.inputType = EInputType::Digital;
        context.inputValue = Vector4(0.f, 0.f, 0.f, 0.f);
        
        for(const InputModifier* inputModifier : m_InputModifiers)
        {
            inputModifier->Modify(context.inputValue, context.inputType);
        }

        context.bIsPressed = false;
        context.bIsRepeat = _bIsRepeat;
        context.controllerId = _controllerId;
        m_InputPerformedEvent.Broadcast(context);
    }

}