#pragma once
#include "Window/Window.h"

struct GLFWwindow;

namespace Nit
{
    class RenderingContext;

    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const Configuration& config = {});
        WindowsWindow(const Window&) = delete;
        WindowsWindow& operator = (const WindowsWindow&) = delete;

        ~WindowsWindow() override;

        void* GetHandler() const override { return static_cast<void*>(m_WindowHandler); }

        WindowEvents& Events() override { return m_Events; }

        void SetTitle(const String title) override;
        const String& GetTitle() const override { return m_Config.Title; }
        
        uint32_t GetWidth() const override { return m_Config.Width; };
        uint32_t GetHeight() const override { return m_Config.Height; };
        
        float GetAspect() const override { return m_Config.Aspect; }
        
        Vector2 GetSize() const override
        {
            const Vector2 windowSize = {
               static_cast<float>(GetWidth()),
               static_cast<float>(GetHeight())
            };
            return windowSize;
        }

        void SetVSync(bool enabled) override;
        bool IsVSync() const override { return m_Config.VSync; };
        
        void SetCursorMode(const CursorMode mode) override;
        CursorMode GetCursorMode() const override { return m_Config.CursorMode; };
    
        bool IsOpened() const override;
        void Close() override;

        void Update();

    private:
        void SetWindowCallbacks();

        Configuration m_Config;
        GLFWwindow* m_WindowHandler;
        WindowEvents m_Events;
        UniquePtr<RenderingContext> m_RenderContext;
        bool m_IsOpened;
    };
}