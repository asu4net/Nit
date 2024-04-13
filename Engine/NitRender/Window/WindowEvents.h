#pragma once

#ifdef NIT_WINDOW_EVENTS

struct GLFWwindow;

namespace Nit::Render
{
    NIT_DECLARE_EVENT_TWO_PARAM(class WindowEvents, WindowResizeEvent, int, width, int, height)
    NIT_DECLARE_EVENT(class WindowEvents, WindowCloseEvent);
    NIT_DECLARE_EVENT_TWO_PARAM(class WindowEvents, WindowKeyPressedEvent, int, key, bool, repeat)
    NIT_DECLARE_EVENT_ONE_PARAM(class WindowEvents, WindowKeyReleasedEvent, int, key)
    NIT_DECLARE_EVENT_ONE_PARAM(class WindowEvents, WindowCursorPosEvent, const Vector2&, pos)
    NIT_DECLARE_EVENT_TWO_PARAM(class WindowEvents, WindowMouseButtonPressedEvent, int, mouseButton, bool, repeat)
    NIT_DECLARE_EVENT_ONE_PARAM(class WindowEvents, WindowMouseButtonReleasedEvent, int, mouseButton)
    NIT_DECLARE_EVENT_ONE_PARAM(class WindowEvents, WindowScrollEvent, const Vector2&, offset)

    class WindowEvents
    {
    public:
        WindowEvents() = default;
        ~WindowEvents()
        {
            ResizeEvent.RemoveAll();
            CloseEvent.RemoveAll();
            KeyPressedEvent.RemoveAll();
            KeyReleasedEvent.RemoveAll();
            CursorPosEvent.RemoveAll();
            MouseButtonPressedEvent.RemoveAll();
            MouseButtonReleasedEvent.RemoveAll();
            ScrollEvent.RemoveAll();
        }

        WindowResizeEvent ResizeEvent;
        WindowCloseEvent CloseEvent;
        WindowKeyPressedEvent KeyPressedEvent;
        WindowKeyReleasedEvent KeyReleasedEvent;
        WindowCursorPosEvent CursorPosEvent;
        WindowMouseButtonPressedEvent MouseButtonPressedEvent;
        WindowMouseButtonReleasedEvent MouseButtonReleasedEvent;
        WindowScrollEvent ScrollEvent;

    protected:
        void CallResizeEvent(const int width, const int height) { ResizeEvent.Broadcast(width, height); }
        void CallCloseEvent() { CloseEvent.Broadcast(); }
        void CallKeyPressedEvent(const int key, const bool repeat) { KeyPressedEvent.Broadcast(key, repeat); }
        void CallKeyReleasedEvent(const int key) { KeyReleasedEvent.Broadcast(key); }
        void CallCursorPosEvent(const Vector2& pos) { CursorPosEvent.Broadcast(pos); }
        void CallMouseButtonPressedEvent(const int mouseButton, const bool repeat) { MouseButtonPressedEvent.Broadcast(mouseButton, repeat); }
        void CallMouseButtonReleasedEvent(const int mouseButton) { MouseButtonReleasedEvent.Broadcast(mouseButton); }
        void CallScrollEvent(const Vector2& offset) { ScrollEvent.Broadcast(offset); }

        friend void WindowSizeCallback(GLFWwindow*, int, int);
        friend void WindowCloseCallback(GLFWwindow*);
        friend void WindowKeyPressedCallback(GLFWwindow*, int, bool);
        friend void WindowKeyReleasedCallback(GLFWwindow*, int);
        friend void WindowCursorPosCallback(GLFWwindow*, const Vector2&);
        friend void WindowMouseButtonPressedCallback(GLFWwindow*, int, bool);
        friend void WindowMouseButtonReleasedCallback(GLFWwindow*, int);
        friend void WindowScrollCallback(GLFWwindow*, const Vector2&);

        friend class Window;
    };
}

#endif