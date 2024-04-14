#pragma once

namespace Nit
{
    class CRenderContext
    {
    public:
        static TUniquePtr<CRenderContext> Create(EGraphicsAPI api, void* windowHandler);
        
        virtual void SwapBuffers() const = 0;
    };
}