#pragma once
#include "Time.h"

namespace Nit
{
    class EngineLayer
    {
    protected:
        EngineLayer() = default;
        virtual ~EngineLayer() = default; 
        
        virtual void OnStart() {}
        virtual void OnUpdate(const TimeStep& timeStep) {}
        virtual void OnFixedUpdate(const TimeStep& timeStep) {}
        virtual void OnFinish() {}

        friend class LayerStack;
    };
}