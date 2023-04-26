#include "Input.h"
#include <glfw/glfw3.h>

namespace Nit 
{
    bool Input::m_Initialized = false;
    void* Input::m_Window = nullptr;
    
    bool Input::IsKeyPressed(const int key)
    {
        if (!m_Initialized) return false;
        auto* window = static_cast<GLFWwindow*>(m_Window);
        const auto state = glfwGetKey(window, key);
        return state == GLFW_PRESS;
    }

    void Input::Initialize(void* window)
    {
        m_Window = window;
        m_Initialized = true;
    }

    bool Input::IsMouseButtonPressed(const int button)
    {
        if (!m_Initialized) return false;
        auto* window = static_cast<GLFWwindow*>(m_Window);
        const auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePosition()
    {
        if (!m_Initialized) return {};
        auto* window = static_cast<GLFWwindow*>(m_Window);
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return { static_cast<float>(xPos), static_cast<float>(yPos) };
    }

    float Input::GetMouseX()
    {
        return GetMousePosition().x;
    }

    float Input::GetMouseY()
    {
        return GetMousePosition().y;
    }
}