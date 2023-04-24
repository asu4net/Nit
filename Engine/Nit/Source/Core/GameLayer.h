#pragma once
#include "Time.h"

namespace Nit
{
    class GameLayer
    {
    protected:
        GameLayer() = default;
        virtual ~GameLayer() = default; 

        virtual void OnCreate() {}
        virtual void OnUpdate(const TimeStep& timeStep) {}
        virtual void OnFixedUpdate(const TimeStep& timeStep) {}
        virtual void OnDestroy() {}

        friend class LayerStack;
    };
}