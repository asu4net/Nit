#pragma once
#include <Nit.h>

namespace Nit
{
    class Editor : public EngineLayer
    {
    public:
        Editor();

        void OnStart() override;
        void OnUpdate(const TimeStep& timeStep) override;
    };
}