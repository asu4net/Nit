#include "Input.h"
#include <GLFW/glfw3.h>
#include "Window/Window.h"
#include "InputAction.h"

namespace Nit::Input
{
    GLFWwindow* m_WindowHandler = nullptr;
    bool m_IsEnabled = true;

    void Init(const UniquePtr<Window>& window)
    {
        m_WindowHandler = static_cast<GLFWwindow*>(window->GetHandler());
        NIT_LOG_TRACE("Input initialized!\n");
    }

    bool IsKeyPressed(uint32_t key)
    {
        if (!m_IsEnabled) return false;
        const auto state = glfwGetKey(m_WindowHandler, key);
        return state == GLFW_PRESS;
    }

    void SetEnabled(const bool bConsumedByEditor)
    {
        m_IsEnabled = bConsumedByEditor;
    }

    bool IsEnabled()
    {
        return m_IsEnabled;
    }

    bool IsMouseButtonPressed(const int button)
    {
        if (!m_IsEnabled) return false;
        const auto state = glfwGetMouseButton(m_WindowHandler, button);
        return state == GLFW_PRESS;
    }

    Vector2 GetMousePosition()
    {
        double xPos, yPos;
        glfwGetCursorPos(m_WindowHandler, &xPos, &yPos);
        return { static_cast<float>(xPos), static_cast<float>(yPos) };
    }
}