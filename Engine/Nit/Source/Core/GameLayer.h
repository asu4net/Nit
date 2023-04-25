#pragma once
#include "Time.h"

namespace Nit
{
    class GameLayer
    {
    protected:
        GameLayer() = default;
        virtual ~GameLayer() = default; 
        
        virtual void OnInitialize() {}
        virtual void OnUpdate(const TimeStep& timeStep) {}
        virtual void OnFixedUpdate(const TimeStep& timeStep) {}
        virtual void OnFinalize() {}

        friend class LayerStack;
    };
}