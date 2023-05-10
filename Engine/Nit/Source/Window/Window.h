#pragma once
#include "WindowEvents.h"

//TODO: Add controller support
//TODO: Add window icon support
//TODO: Add overload to CreateRef

namespace Nit
{
    class RenderingContext;
    
    class Window
    {
    public:
        enum class CursorMode { Normal, Disabled, Hidden, Captured };
        
        struct Configuration
        {
            std::string Title{"Nit Window"};
            int Width{1280}, Height{720};
            int OpenGlMajorVersion{4}, OpenGlMinorVersion{6};
            bool VSync{true};
            CursorMode CursorMode{CursorMode::Normal};
        };
        
        static Shared<Window> Create();
        
        void Start(const Configuration& config = {});
        void Finish();
        
        WindowEvents& Events() { return m_Events; }
        void Close() { m_KeepWindowOpened = false; }
        bool IsOpened();
        
        void SetVSync(bool enabled);
        bool IsVSync() const { return m_Config.VSync; }

        void SetTitle(const std::string& title);
        const std::string& GetTitle() const { return m_Config.Title; }
        
        void SetCursorMode(const CursorMode mode);
        CursorMode GetCursorMode() const { return m_Config.CursorMode; }
        
        uint32_t GetWidth() const { return m_Config.Width; }
        uint32_t GetHeight() const { return m_Config.Height; }
        float GetAspect() const { return static_cast<float>(GetWidth()) / static_cast<float>(GetHeight()); }

        void* GetHandler() const { return m_WindowHandler; }

        std::string OpenFile(const std::string& filter);
        std::string SaveFile(const std::string& filter);
        
        virtual void Update();

        
    private:
        Shared<RenderingContext> m_Context;
        WindowEvents m_Events;
        GLFWwindow* m_WindowHandler = nullptr;    
        Configuration m_Config;
        bool m_bCreated = false;
        bool m_KeepWindowOpened = true;
        
        virtual void SetWindowCallbacks();
    };
}