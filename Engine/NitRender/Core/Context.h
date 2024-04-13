#pragma once

namespace Nit::Render
{
    class Context
    {
    public:
        static UniquePtr<Context> Create(GraphicsAPI api, void* windowHandler);
        
        virtual void SwapBuffers() const = 0;
    };
}