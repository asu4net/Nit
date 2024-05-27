#pragma once

namespace Nit
{
    class InputModifier;

    enum class EInputType
    {
        Digital,
        Axis1D,
        Axis2D,
        Axis3D,
        Axis4D
    };

    enum class ETriggerType
    {
        Down,
        Pressed,
        Released
    };
    
    
    struct InputActionContext
    {
        bool bIsRepeat  = false;
        bool bIsPressed = false;
        EInputType inputType;
        Vector4 inputValue;
        int controllerId = -1;
    };

    NIT_DECLARE_EVENT_ONE_PARAM(class InputAction, InputPerformedEvent, const InputActionContext&, context)

    class InputAction
    {
    public:
        InputAction(String keyCode);

        ~InputAction();

        InputPerformedEvent& OnPerformed() { return m_InputPerformedEvent; }
        bool IsKeyPressed() const { return m_IsKeyPressed; }
        const String& GetKeyCode() const { return m_KeyCode; }
        ETriggerType GetTriggerType() const { return m_TriggerType; }
        void SetTriggerType(ETriggerType _triggerType) { m_TriggerType = _triggerType; }
        void AddInputModifier(InputModifier* _inputModifier) { m_InputModifiers.push_back(_inputModifier); }

        void Update();

        void OnControllerAnalog(int _controllerId, float _analogValue);
        void OnControllerVector2(int _controllerId, const Vector2& _vector2Value);
        void OnControllerVector3(int _controllerId, const Vector3& _vector3Value);
        void OnControllerVector4(int _controllerId, const Vector4& _vector4Value);
        void OnControllerButtonPressed(int _controllerId, bool _bIsRepeat);
        void OnControllerButtonReleased(int _controllerId, bool _bIsRepeat);
    private:
        InputPerformedEvent m_InputPerformedEvent;
        const String m_KeyCode;
        bool m_IsKeyPressed = false;
        ETriggerType m_TriggerType = ETriggerType::Pressed;
        DynamicArray<InputModifier*> m_InputModifiers;
    };
}