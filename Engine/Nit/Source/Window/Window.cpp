#include "Window.h"
#include <glfw/glfw3.h>

#include "Input/Input.h"
#include "Rendering/RenderingContext.h"

namespace Nit
{
    std::shared_ptr<Window> Window::CreateAndInitialize(const Configuration& config)
    {
        auto window = Window::Create();
        window->Initialize(config);
        return window;
    }

    std::shared_ptr<Window> Window::Create()
    {
        return std::make_shared<Window>();
    }

    void Window::Initialize(const Configuration& config)
    {
        assert(!m_bCreated && "Window already created!");
        Events().CallBeginInitializeEvent();
        m_Config = config;
        
        assert(glfwInit() && "GLFW initialisation failed!");
        
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_Config.OpenGlMajorVersion);
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_Config.OpenGlMinorVersion);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
        
        m_WindowHandler = glfwCreateWindow(m_Config.Width, m_Config.Height, m_Config.Title.c_str(), nullptr, nullptr);
        assert(m_WindowHandler && "Window creation failed!");
        
        m_Context = InstantiateContext(m_WindowHandler);
        glfwMakeContextCurrent(m_WindowHandler);
        
        m_Context->Initialize();
        
        SetVSync(config.VSync);

        Events().ResizeEvent.Add([&](const int width, const int height)
        {
            m_Config.Width = width;
            m_Config.Height = height;
        });
        
        SetWindowCallbacks();
        Input::Initialize(GetHandler());
        
        Events().CallEndInitializeEvent();
        m_bCreated = true;
    }
    
    void Window::Finalize()
    {
        Events().CallBeginFinalizeEvent();
        assert(m_bCreated && "You're trying to destroy an uncreated window!");
        glfwDestroyWindow(m_WindowHandler);
        m_bCreated = false;
        Events().CallEndFinalizeEvent();
    }

    bool Window::IsOpened()
    {
        return !glfwWindowShouldClose(m_WindowHandler) && m_KeepWindowOpened;
    }

    void Window::SetVSync(const bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
        m_Config.VSync = enabled;
    }

    void Window::SetTitle(const std::string& title)
    {
        m_Config.Title = title;
        glfwSetWindowTitle(m_WindowHandler, title.c_str());
    }
    
    void Window::SetCursorMode(const CursorMode mode)
    {
        m_Config.CursorMode = mode;
		
        switch (m_Config.CursorMode)
        {
        case CursorMode::Normal:
            glfwSetInputMode(m_WindowHandler, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            return;
        case CursorMode::Disabled:
            glfwSetInputMode(m_WindowHandler, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            return;
        case CursorMode::Hidden:
            glfwSetInputMode(m_WindowHandler, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            return;
        case CursorMode::Captured:
            glfwSetInputMode(m_WindowHandler, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
        }
    }
    
    //---------------------------------------------------------
    // GLFW CALLBACKS
    //---------------------------------------------------------
    
    static WindowEvents& GetEvents(GLFWwindow* windowHandler)
    {
        static WindowEvents* events = static_cast<WindowEvents*>(glfwGetWindowUserPointer(windowHandler));
        return *events;
    }
    
    void WindowSizeCallback(GLFWwindow* windowHandler, const int width, const int height) { GetEvents(windowHandler).CallResizeEvent(width, height); }
    void WindowCloseCallback(GLFWwindow* windowHandler) { GetEvents(windowHandler).CallCloseEvent(); }
    void WindowKeyPressedCallback(GLFWwindow* windowHandler, const int key, const bool pressed) { GetEvents(windowHandler).CallKeyPressedEvent(key, pressed); }
    void WindowKeyReleasedCallback(GLFWwindow* windowHandler, const int key) { GetEvents(windowHandler).CallKeyReleasedEvent(key); }
    void WindowCursorPosCallback(GLFWwindow* windowHandler, const glm::vec2& pos) { GetEvents(windowHandler).CallCursorPosEvent(pos); }
    void WindowMouseButtonPressedCallback(GLFWwindow* windowHandler, const int mouseButton, const bool pressed) { GetEvents(windowHandler).CallMouseButtonPressedEvent(mouseButton, pressed); }
    void WindowMouseButtonReleasedCallback(GLFWwindow* windowHandler, const int mouseButton) { GetEvents(windowHandler).CallMouseButtonReleasedEvent(mouseButton); }
    void WindowScrollCallback(GLFWwindow* windowHandler, const glm::vec2& offset) { GetEvents(windowHandler).CallScrollEvent(offset); }

    void Window::Update()
    {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void Window::SetWindowCallbacks()
    {
        glfwSetWindowUserPointer(m_WindowHandler, &m_Events);
        
        glfwSetWindowSizeCallback(m_WindowHandler, WindowSizeCallback);
        glfwSetWindowCloseCallback(m_WindowHandler, WindowCloseCallback);
        glfwSetKeyCallback(m_WindowHandler, [](GLFWwindow* windowHandler, const int key, const int scanCode, const int action, const int mods)
        {
            switch (action)
            {
            case GLFW_PRESS:
                WindowKeyPressedCallback(windowHandler, key, false);
                return;
            case GLFW_RELEASE:
                WindowKeyReleasedCallback(windowHandler, key);
                return;
            case GLFW_REPEAT:
                WindowKeyPressedCallback(windowHandler, key, true);
            }
        });
        glfwSetCursorPosCallback(m_WindowHandler, [](GLFWwindow* windowHandler, const double xPos, const double yPos)
        {
            const glm::vec2 cursorPosition = {static_cast<float>(xPos), static_cast<float>(yPos)};
            WindowCursorPosCallback(windowHandler, cursorPosition);
        });
        glfwSetMouseButtonCallback(m_WindowHandler, [](GLFWwindow* windowHandler, const int button, const int action, const int mods)
        {
            switch (action)
            {
            case GLFW_PRESS:
                WindowMouseButtonPressedCallback(windowHandler, button, false);
                return;
            case GLFW_RELEASE:
                WindowMouseButtonReleasedCallback(windowHandler, button);
                return;
            case GLFW_REPEAT:
                WindowMouseButtonPressedCallback(windowHandler, button, true);
            }
        });
        glfwSetScrollCallback(m_WindowHandler, [](GLFWwindow* windowHandler, const double xOffset, const double yOffset)
        {
            const glm::vec2 offset = {static_cast<float>(xOffset), static_cast<float>(yOffset)};
            WindowScrollCallback(windowHandler, offset);
        });
    }

    // ---------------------------------------------------------
}