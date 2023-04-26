#pragma once
#include "GameLayer.h"

//TODO: Pop and Get methods. Requirements: rtti

namespace Nit
{
    class LayerStack
    {
    public:
        LayerStack() = default;
        
        template<typename T, typename ...TArgs>
        std::shared_ptr<T> Push(TArgs&& ...args)
        {
            std::shared_ptr<T> gameLayer = std::make_shared<T>(std::forward<TArgs>(args)...);
            m_GameLayers.push_back(gameLayer);
            return gameLayer;
        }

        template<typename T>
        std::shared_ptr<T> Get()
        {
            for (std::shared_ptr<GameLayer> gameLayer : m_GameLayers)
            {
                if (std::shared_ptr<T> castedLayer = std::dynamic_pointer_cast<T>(gameLayer))
                    return castedLayer;
            }
            return nullptr;
        }

        void Initialize();
        void Update(const TimeStep& timeStep);
        void Finalize();

    private:
        std::vector<std::shared_ptr<GameLayer>> m_GameLayers;
    };
}