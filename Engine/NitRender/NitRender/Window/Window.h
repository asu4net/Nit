#pragma once

#include "WindowEvents.h"

namespace Nit
{
    enum class ECursorMode { Normal, Disabled, Hidden, Captured };
    
    class CWindow
    {
    public:
        struct CConfiguration
        {
            TString Title          = "Nit Window";
            int Width             = 1920;
            int Height            = 1080 ;
            float Aspect          = (float) Width / (float) Height;
            EGraphicsAPI API       = EGraphicsAPI::OpenGL;
            bool VSync            = true;
            bool bStartMaximized  = false;
            ECursorMode CursorMode = ECursorMode::Normal;
        };

        virtual ~CWindow() = default;

        virtual void* GetHandler() const = 0;

#ifdef NIT_WINDOW_EVENTS
        virtual WindowEvents& Events() = 0;
#endif
        virtual void SetTitle(const TString title) = 0;
        virtual const TString& GetTitle() const = 0;

        virtual uint32_t GetWidth() const = 0;
        
        virtual uint32_t GetHeight() const = 0;

        virtual float GetAspect() const = 0;

        virtual CVector2 GetSize() const = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void SetCursorMode(const ECursorMode mode) = 0;
        virtual ECursorMode GetCursorMode() const = 0;
        
        virtual bool IsOpened() const = 0;
                
        virtual void Close() = 0;

        virtual void Update() = 0;

        static TUniquePtr<CWindow> Create(const CConfiguration& config = {});
    };

    using TWindowPtr = TUniquePtr<CWindow>;
}