#include "LayerStack.h"

namespace Nit
{
    void LayerStack::Start()
    {
        for (const auto& gameLayer : m_GameLayers)
            gameLayer->OnStart();
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

        for (const auto& gameLayer : m_GameLayers)
            gameLayer->OnPostFixedUpdate(timeStep);
    }

    void LayerStack::Finish()
    {
        for (const auto& gameLayer : m_GameLayers)
            gameLayer->OnFinish();
    }
}