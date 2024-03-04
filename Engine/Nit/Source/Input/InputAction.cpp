#include "InputAction.h"
#include "Core\Engine.h"

namespace Nit
{
    InputAction::InputAction(KeyCode keyCode)
        : m_KeyCode(keyCode)
    {}

    InputAction::InputAction() = default;
    InputAction::~InputAction() 
    {
        m_InputPerformedEvent.RemoveAll();
    };

    void InputAction::Update()
    {
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
    }
}