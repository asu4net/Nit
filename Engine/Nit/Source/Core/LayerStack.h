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
        Shared<T> Push(TArgs&& ...args)
        {
            Shared<T> gameLayer = std::make_shared<T>(std::forward<TArgs>(args)...);
            m_GameLayers.push_back(gameLayer);
            return gameLayer;
        }

        template<typename T>
        Shared<T> Get()
        {
            for (Shared<GameLayer> gameLayer : m_GameLayers)
            {
                if (Shared<T> castedLayer = std::dynamic_pointer_cast<T>(gameLayer))
                    return castedLayer;
            }
            return nullptr;
        }

        void Initialize();
        void Update(const TimeStep& timeStep);
        void Finalize();

    private:
        std::vector<Shared<GameLayer>> m_GameLayers;
    };
}