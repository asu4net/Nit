#pragma once
#include "Asset.h"

namespace Nit
{
    class AssetManager : public Singleton<AssetManager>
    {
    public:
        void Initialize();
        void Finalize();
    private:
        AssetManager() = default;

        template<typename T>
        Weak<T> GetFromId(const Id id)
        {
            if (!m_IdAssetMap.contains(id))
                return nullptr;
            return std::dynamic_pointer_cast<T>(m_IdAssetMap[id]);
        }
        
        std::unordered_map<Id, Shared<Asset>> m_IdAssetMap;
        
        friend class Singleton<AssetManager>;
    };
}
