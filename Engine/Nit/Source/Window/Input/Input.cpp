#include "Input.h"
#include <glfw/glfw3.h>

namespace Nit 
{
    bool Input::m_bInitialized = false;
    void* Input::m_Window = nullptr;
    bool Input::m_bConsumedByEditor = false;
    
    bool Input::IsKeyPressed(const int key)
    {
        if (!m_bInitialized || m_bConsumedByEditor) return false;
        auto* window = static_cast<GLFWwindow*>(m_Window);
        const auto state = glfwGetKey(window, key);
        return state == GLFW_PRESS;
    }

    void Input::SetConsumedByEditor(const bool bConsumedByEditor)
    {
        m_bConsumedByEditor = bConsumedByEditor;
    }

    bool Input::IsConsumedByEditor()
    {
        return m_bConsumedByEditor;
    }

    void Input::Initialize(void* window)
    {
        m_Window = window;
        m_bInitialized = true;
    }

    bool Input::IsMouseButtonPressed(const int button)
    {
        if (!m_bInitialized || m_bConsumedByEditor) return false;
        auto* window = static_cast<GLFWwindow*>(m_Window);
        const auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    Vec2 Input::GetMousePosition()
    {
        if (!m_bInitialized) return {};
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