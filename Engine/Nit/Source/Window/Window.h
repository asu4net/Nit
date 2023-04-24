#pragma once
#include <memory>
#include <string>
#include "WindowEvents.h"
#include "Math/Math.h"

//TODO: Add controller support
//TODO: Add window icon support

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
            glm::vec4 Color{Global::DarkGreyColor};
            int OpenGlMajorVersion{4}, OpenGlMinorVersion{6};
            bool VSync{true};
            CursorMode CursorMode{CursorMode::Normal};
        };
        
        virtual ~Window() = default;
        
        static std::shared_ptr<Window> CreateAndInitialize(const Configuration& config = {});
        static std::shared_ptr<Window> Create();
        
        virtual void Initialize(const Configuration& config = {});
        virtual void Finalize();
        
        virtual WindowEvents& Events() { return m_Events; }
        virtual void Close() { m_KeepWindowOpened = false; }
        virtual bool IsOpened();
        
        virtual void SetVSync(bool enabled);
        virtual bool IsVSync() const { return m_Config.VSync; }

        virtual void SetTitle(const std::string& title);
        virtual const std::string& GetTitle() const { return m_Config.Title; }
        
        virtual void SetCursorMode(const CursorMode mode);
        virtual CursorMode GetCursorMode() const { return m_Config.CursorMode; }
        
        virtual void SetBackgroundColor(const glm::vec4& color) { m_Config.Color = color; }
        virtual glm::vec4 GetBackgroundColor() const { return m_Config.Color; }
        
        virtual uint32_t GetWidth() const { return m_Config.Width; }
        virtual uint32_t GetHeight() const { return m_Config.Height; }
        virtual float GetAspect() const { return static_cast<float>(GetWidth()) / static_cast<float>(GetHeight()); }

        void* GetHandler() const { return m_WindowHandler; }

        virtual void Update();

        
    private:
        std::shared_ptr<RenderingContext> m_Context;
        WindowEvents m_Events;
        GLFWwindow* m_WindowHandler = nullptr;    
        Configuration m_Config;
        bool m_bCreated = false;
        bool m_KeepWindowOpened = true;
        
        virtual void SetWindowCallbacks();
    };
}