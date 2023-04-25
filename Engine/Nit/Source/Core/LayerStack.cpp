#include "LayerStack.h"

namespace Nit
{
    void LayerStack::Initialize()
    {
        for (const auto& gameLayer : m_GameLayers)
            gameLayer->OnInitialize();
    }

    void LayerStack::Update(const TimeStep& timeStep)
    {
        for (const auto& gameLayer : m_GameLayers)
            gameLayer->OnUpdate(timeStep);

        for (int i = 0; i < timeStep.FixedUpdateCalls; i++)
        {
            for (const auto& gameLayer : m_GameLayers)
                gameLayer->OnFixedUpdate(timeStep);
        }
    }

    void LayerStack::Finalize()
    {
        for (const auto& gameLayer : m_GameLayers)
            gameLayer->OnFinalize();
    }
}