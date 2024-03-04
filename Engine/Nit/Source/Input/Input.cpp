#include "Input.h"
#include <GLFW/glfw3.h>
#include "Window/Window.h"
#include "InputAction.h"

namespace Nit
{
    static GLFWwindow* m_WindowHandler = nullptr;

    void Input::Init(const UniquePtr<Window>& window)
    {
        m_WindowHandler = static_cast<GLFWwindow*>(window->GetHandler());
        NIT_LOG_TRACE("Input initialized!\n");
    }

    bool Input::IsKeyPressed(KeyCode keyCode)
    {
        return IsKeyPressed(static_cast<uint32_t>(keyCode));
    }

    bool Input::IsKeyPressed(uint32_t key)
    {
        if (!m_IsEnabled) return false;
        const auto state = glfwGetKey(m_WindowHandler, key);
        return state == GLFW_PRESS;
    }

    void Input::SetEnabled(const bool bConsumedByEditor)
    {
        m_IsEnabled = bConsumedByEditor;
    }

    bool Input::IsEnabled()
    {
        return m_IsEnabled;
    }

    bool Input::IsMouseButtonPressed(const int button)
    {
        if (!m_IsEnabled) return false;
        const auto state = glfwGetMouseButton(m_WindowHandler, button);
        return state == GLFW_PRESS;
    }

    Vector2 Input::GetMousePosition()
    {
        double xPos, yPos;
        glfwGetCursorPos(m_WindowHandler, &xPos, &yPos);
        return { static_cast<float>(xPos), static_cast<float>(yPos) };
    }

    WeakPtr<InputAction> Input::RegisterInputAction(KeyCode keyCode)
    {
        SharedPtr<InputAction> inputAction = CreateSharedPtr<InputAction>(keyCode);
        m_InputActions.push_back(inputAction);
        return inputAction;
    }

    bool Input::UnregisterInputAction(WeakPtr<InputAction> inputAction)
    {
        if (inputAction.expired())
        {
            return false;
        }

        auto it = std::find(m_InputActions.begin(), m_InputActions.end(), inputAction.lock());

        if (!*it)
        {
            return false;
        }

        m_InputActions.erase(it);
        return true;
    }

    void Input::UpdateInputActions()
    {
        if (!m_IsEnabled) return;

        for (auto inputAction : m_InputActions)
        {
            inputAction->Update();
        }
    }
}