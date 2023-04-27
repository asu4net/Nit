#pragma once
#include "Asset.h"

namespace Nit
{
    class AssetManager : public Singleton<AssetManager>
    {
    public:
        template<typename T>
                Weak<T> GetFromId(const Id id)
        {
            if (!m_IdAssetMap.contains(id))
                return nullptr;
            return std::dynamic_pointer_cast<T>(m_IdAssetMap[id]);
        }
        
        void Initialize();
        void Finalize();
    
    private:
        std::unordered_map<Id, Shared<Asset>> m_IdAssetMap;

        AssetManager() = default;
        
        Id SaveAsset(const std::string& path, const std::string& type, const std::string& name);
        
        friend class Singleton<AssetManager>;
    };
}
