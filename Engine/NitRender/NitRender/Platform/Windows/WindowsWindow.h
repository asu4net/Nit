// ReSharper disable CppInconsistentNaming
#pragma once
#include "NitRender/Window/Window.h"

struct GLFWwindow;

namespace Nit
{
    class CRenderContext;

    class CWindowsWindow : public CWindow
    {
    public:
        CWindowsWindow(const CConfiguration& config = {});
        CWindowsWindow(const CWindow&) = delete;
        CWindowsWindow& operator = (const CWindowsWindow&) = delete;

        ~CWindowsWindow() override;

        void* GetHandler() const override { return static_cast<void*>(m_WindowHandler); }

#ifdef NIT_WINDOW_EVENTS
        WindowEvents& Events() override { return m_Events; }
#endif
        
        void SetTitle(const TString title) override;
        const TString& GetTitle() const override { return m_Config.Title; }
        
        uint32_t GetWidth() const override { return m_Config.Width; };
        uint32_t GetHeight() const override { return m_Config.Height; };
        
        float GetAspect() const override { return m_Config.Aspect; }
        
        CVector2 GetSize() const override
        {
            const CVector2 windowSize = {
               static_cast<float>(GetWidth()),
               static_cast<float>(GetHeight())
            };
            return windowSize;
        }

        void SetVSync(bool enabled) override;
        bool IsVSync() const override { return m_Config.VSync; };
        
        void SetCursorMode(const ECursorMode mode) override;
        ECursorMode GetCursorMode() const override { return m_Config.CursorMode; };
    
        bool IsOpened() const override;
        void Close() override;

        void Update() override;

    private:
#ifdef NIT_WINDOW_EVENTS
        void SetWindowCallbacks();
#endif
        CConfiguration m_Config;
        GLFWwindow* m_WindowHandler = nullptr;
#ifdef NIT_WINDOW_EVENTS
        WindowEvents m_Events;
#endif
        TUniquePtr<CRenderContext> m_RenderContext = nullptr;
        bool m_IsOpened = false;
    };
}