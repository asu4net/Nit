#pragma once

namespace Nit
{
    struct InputActionContext
    {
        bool IsPressed  = false;
        bool IsReleased = false;
        int  KeyCode  = 0;
    };

    NIT_DECLARE_EVENT_ONE_PARAM(class InputAction, InputPerformedEvent, const InputActionContext&, context)

    class InputAction
    {
    public:
        InputAction(KeyCode keyCode);
        InputAction();

        ~InputAction();

        InputPerformedEvent& OnPerformed() { return m_InputPerformedEvent; }
        bool IsKeyPressed() const { return m_IsKeyPressed; }
        KeyCode GetKeyCode() const { return m_KeyCode; }
        void SetKeyCode(KeyCode keyCode) { m_KeyCode = keyCode; }

        void Update();

    private:
        InputPerformedEvent m_InputPerformedEvent;
        KeyCode m_KeyCode = Key_Invalid;
        bool m_IsKeyPressed = false;
    };
}