#pragma once
#include "EngineLayer.h"

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
            for (Shared<EngineLayer> gameLayer : m_GameLayers)
            {
                if (Shared<T> castedLayer = std::dynamic_pointer_cast<T>(gameLayer))
                    return castedLayer;
            }
            return nullptr;
        }

        void Start();
        void Update(const TimeStep& timeStep);
        void Finish();

    private:
        std::vector<Shared<EngineLayer>> m_GameLayers;
    };
}