#include "Input.h"
#include <GLFW/glfw3.h>

namespace Nit::Input
{
    GLFWwindow* m_WindowHandler = nullptr;
    bool m_IsEnabled = true;

    void Init(const TWindowPtr& window)
    {
        m_WindowHandler = static_cast<GLFWwindow*>(window->GetHandler());
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

    CVector2 GetMousePosition()
    {
        double xPos, yPos;
        glfwGetCursorPos(m_WindowHandler, &xPos, &yPos);
        return { static_cast<float>(xPos), static_cast<float>(yPos) };
    }
}