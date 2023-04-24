#pragma once
#include <memory>
#include <vector>
#include "GameLayer.h"

namespace Nit
{
    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();
        
        template<typename T, typename ...TArgs>
        std::shared_ptr<T> PushLayer(TArgs&& ...args)
        {
            std::shared_ptr<T> gameLayer = std::make_shared<T>(std::forward<TArgs>(args)...);
            m_GameLayers.push_back(gameLayer);
            gameLayer->OnCreate();
            return gameLayer;
        }

        void UpdateLayers(const TimeStep& timeStep);

    private:
        std::vector<std::shared_ptr<GameLayer>> m_GameLayers;
    };
}
