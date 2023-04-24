#include "LayerStack.h"

namespace Nit
{
    LayerStack::~LayerStack()
    {
        for (const auto& gameLayer : m_GameLayers)
            gameLayer->OnDestroy();
    }

    void LayerStack::UpdateLayers(const TimeStep& timeStep)
    {
        for (const auto& gameLayer : m_GameLayers)
            gameLayer->OnUpdate(timeStep);

        for (int i = 0; i < timeStep.FixedUpdateCalls; i++)
        {
            for (const auto& gameLayer : m_GameLayers)
                gameLayer->OnFixedUpdate(timeStep);
        }
    }
}
