#include "WindowsWindow.h"
#include "NitRender/Core/RenderContext.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Nit
{
    void SetGlfwWindowCursorMode(GLFWwindow* handler, ECursorMode mode)
    {
        switch (mode)
        {
        case ECursorMode::Normal:
            glfwSetInputMode(handler, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            return;
        case ECursorMode::Disabled:
            glfwSetInputMode(handler, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            return;
        case ECursorMode::Hidden:
            glfwSetInputMode(handler, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            return;
        case ECursorMode::Captured:
            glfwSetInputMode(handler, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
        }
    }
    
    CWindowsWindow::CWindowsWindow(const CConfiguration& config)
        : m_Config(config)
    {
        const bool bSuccess = glfwInit();
        assert(bSuccess && "Window initialization failed!");
        if (!bSuccess) return;

        m_WindowHandler = glfwCreateWindow(m_Config.Width, m_Config.Height, m_Config.Title.c_str(), nullptr, nullptr);
        
        glfwMakeContextCurrent(m_WindowHandler);

        m_RenderContext = CRenderContext::Create(m_Config.API, m_WindowHandler);
        m_IsOpened = true;

        glfwSwapInterval(1);
        m_Config.VSync = true;

        m_Config.Title = config.Title;
        glfwSetWindowTitle(m_WindowHandler, config.Title.c_str());
        
        m_Config.CursorMode = config.CursorMode;
        SetGlfwWindowCursorMode(m_WindowHandler, config.CursorMode);
        
#ifdef NIT_WINDOW_EVENTS
        SetWindowCallbacks();
#endif
        if (m_Config.bStartMaximized)
        {
            glfwSetWindowAttrib(m_WindowHandler, GLFW_MAXIMIZED, GLFW_TRUE);
            glfwMaximizeWindow(m_WindowHandler);
        }

#ifdef WINDOW_EVENTS
        Events().ResizeEvent.Add([&](int width, int height) {
            m_Config.Width = width;
            m_Config.Height = height;
            m_Config.Aspect = (float) width / (float) height;
        });
#endif
    }

    CWindowsWindow::~CWindowsWindow()
    {
        if (m_IsOpened && !glfwWindowShouldClose(m_WindowHandler))
        {
            m_IsOpened = false;
        }
        
        glfwDestroyWindow(m_WindowHandler);
        m_WindowHandler = nullptr;
        glfwTerminate();
    }

    void CWindowsWindow::SetTitle(const TString title)
    {
        m_Config.Title = title;
        glfwSetWindowTitle(m_WindowHandler, title.c_str());
    }

    void CWindowsWindow::SetVSync(bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
        m_Config.VSync = enabled;
    }

    void CWindowsWindow::SetCursorMode(const ECursorMode mode)
    {
        m_Config.CursorMode = mode;
        SetGlfwWindowCursorMode(m_WindowHandler, mode);
    }

    bool CWindowsWindow::IsOpened() const
    {
        return m_IsOpened && !glfwWindowShouldClose(m_WindowHandler);
    }

    void CWindowsWindow::Close()
    {
        m_IsOpened = false;
    }

    void CWindowsWindow::Update()
    {
        glfwPollEvents();
        m_RenderContext->SwapBuffers();
    }

    //---------------------------------------------------------
    // GLFW CALLBACKS
    //---------------------------------------------------------
#ifdef NIT_WINDOW_EVENTS
    static WindowEvents& GetEvents(GLFWwindow* windowHandler)
    {
        static WindowEvents* events = static_cast<WindowEvents*>(glfwGetWindowUserPointer(windowHandler));
        return *events;
    }

    void WindowSizeCallback(GLFWwindow* windowHandler, const int width, const int height) { GetEvents(windowHandler).CallResizeEvent(width, height); }
    void WindowCloseCallback(GLFWwindow* windowHandler) { GetEvents(windowHandler).CallCloseEvent(); }
    void WindowKeyPressedCallback(GLFWwindow* windowHandler, const int key, const bool pressed) { GetEvents(windowHandler).CallKeyPressedEvent(key, pressed); }
    void WindowKeyReleasedCallback(GLFWwindow* windowHandler, const int key) { GetEvents(windowHandler).CallKeyReleasedEvent(key); }
    void WindowCursorPosCallback(GLFWwindow* windowHandler, const Vector2& pos) { GetEvents(windowHandler).CallCursorPosEvent(pos); }
    void WindowMouseButtonPressedCallback(GLFWwindow* windowHandler, const int mouseButton, const bool pressed) { GetEvents(windowHandler).CallMouseButtonPressedEvent(mouseButton, pressed); }
    void WindowMouseButtonReleasedCallback(GLFWwindow* windowHandler, const int mouseButton) { GetEvents(windowHandler).CallMouseButtonReleasedEvent(mouseButton); }
    void WindowScrollCallback(GLFWwindow* windowHandler, const Vector2& offset) { GetEvents(windowHandler).CallScrollEvent(offset); }

    void WindowsWindow::SetWindowCallbacks()
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
                const Vector2 cursorPosition = { static_cast<float>(xPos), static_cast<float>(yPos) };
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
                const Vector2 offset = { static_cast<float>(xOffset), static_cast<float>(yOffset) };
                WindowScrollCallback(windowHandler, offset);
            });
    }

#endif
}