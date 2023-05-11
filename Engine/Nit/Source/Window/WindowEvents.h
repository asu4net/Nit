#pragma once

struct GLFWwindow;

#define MACRO 

namespace Nit
{
    DECLARE_EVENT(class WindowEvents, WindowBeginInitializeEvent);
    DECLARE_EVENT(class WindowEvents, WindowEndInitializeEvent);
    DECLARE_EVENT(class WindowEvents, WindowBeginFinalizeEvent);
    DECLARE_EVENT(class WindowEvents, WindowEndFinalizeEvent);
    DECLARE_EVENT_TWO_PARAM(class WindowEvents, WindowResizeEvent, int, width, int, height)
    DECLARE_EVENT(class WindowEvents, WindowCloseEvent);
    DECLARE_EVENT_TWO_PARAM(class WindowEvents, WindowKeyPressedEvent, int, key, bool, repeat)
    DECLARE_EVENT_ONE_PARAM(class WindowEvents, WindowKeyReleasedEvent, int, key)
    DECLARE_EVENT_ONE_PARAM(class WindowEvents, WindowCursorPosEvent, const vec2&, pos)
    DECLARE_EVENT_TWO_PARAM(class WindowEvents, WindowMouseButtonPressedEvent, int, mouseButton, bool, repeat)
    DECLARE_EVENT_ONE_PARAM(class WindowEvents, WindowMouseButtonReleasedEvent, int, mouseButton)
    DECLARE_EVENT_ONE_PARAM(class WindowEvents, WindowScrollEvent, const vec2&, offset)
    
    class WindowEvents
    {
    public:
        WindowEvents() = default;
        ~WindowEvents()
        {
            BeginInitializeEvent.RemoveAll();
            BeginFinalizeEvent.RemoveAll();
            EndFinalizeEvent.RemoveAll();
            ResizeEvent.RemoveAll();
            CloseEvent.RemoveAll();
            KeyPressedEvent.RemoveAll();
            KeyReleasedEvent.RemoveAll();
            CursorPosEvent.RemoveAll();
            MouseButtonPressedEvent.RemoveAll();
            MouseButtonReleasedEvent.RemoveAll();
            ScrollEvent.RemoveAll();
        }
        
        WindowBeginInitializeEvent BeginInitializeEvent;
        WindowEndInitializeEvent EndInitializeEvent;
        WindowBeginFinalizeEvent BeginFinalizeEvent;
        WindowEndFinalizeEvent EndFinalizeEvent;
        WindowResizeEvent ResizeEvent;
        WindowCloseEvent CloseEvent;
        WindowKeyPressedEvent KeyPressedEvent;
        WindowKeyReleasedEvent KeyReleasedEvent;
        WindowCursorPosEvent CursorPosEvent;
        WindowMouseButtonPressedEvent MouseButtonPressedEvent;
        WindowMouseButtonReleasedEvent MouseButtonReleasedEvent;
        WindowScrollEvent ScrollEvent;
        
    protected:
        void CallBeginInitializeEvent() { BeginInitializeEvent.Broadcast(); }
        void CallEndInitializeEvent() { EndInitializeEvent.Broadcast(); }
        void CallBeginFinalizeEvent() { BeginFinalizeEvent.Broadcast(); }
        void CallEndFinalizeEvent() { EndFinalizeEvent.Broadcast(); }
        void CallResizeEvent(const int width, const int height) { ResizeEvent.Broadcast(width, height); }
        void CallCloseEvent() { CloseEvent.Broadcast(); }
        void CallKeyPressedEvent(const int key, const bool repeat) { KeyPressedEvent.Broadcast(key, repeat); }
        void CallKeyReleasedEvent(const int key) { KeyReleasedEvent.Broadcast(key); }
        void CallCursorPosEvent(const vec2& pos) { CursorPosEvent.Broadcast(pos); }
        void CallMouseButtonPressedEvent(const int mouseButton, const bool repeat) { MouseButtonPressedEvent.Broadcast(mouseButton, repeat); }
        void CallMouseButtonReleasedEvent(const int mouseButton) { MouseButtonReleasedEvent.Broadcast(mouseButton); }
        void CallScrollEvent(const vec2& offset) { ScrollEvent.Broadcast(offset); }

        friend void WindowSizeCallback(GLFWwindow*, int, int);
        friend void WindowCloseCallback(GLFWwindow*);
        friend void WindowKeyPressedCallback(GLFWwindow*, int, bool);
        friend void WindowKeyReleasedCallback(GLFWwindow*, int);
        friend void WindowCursorPosCallback(GLFWwindow*, const vec2&);
        friend void WindowMouseButtonPressedCallback(GLFWwindow*, int, bool);
        friend void WindowMouseButtonReleasedCallback(GLFWwindow*, int);
        friend void WindowScrollCallback(GLFWwindow*, const vec2&);

        friend class Window;
    };
}