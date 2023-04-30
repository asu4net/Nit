#pragma once
#include "Asset.h"
#include "AssetLink.h"

namespace Nit
{
    class AssetManager : public Singleton<AssetManager>
    {
    public:
        template<typename T>
        AssetLink<T> CreateAsset(const std::string& name, const std::string& path)
        {
            return CreateAssetWithId<T>(name, path, Id());
        }
        
        template<typename T>
        AssetLink<T> CreateAssetWithId(const std::string& name, const std::string& path, const Id& id)
        {
            AssetLink<T> link;
            const rttr::type t = rttr::type::get<T>();

            if (!t.is_valid() || !t.is_derived_from<Asset>())
                return link;
            
            if (m_IdAssetMap.contains(id))
            {
                const Shared<Asset> asset = m_IdAssetMap[id];
                link.SetTarget(asset);
                return link;
            }

            Shared<T> asset = CreateShared<T>(name, path, id);
            asset->Load();
            m_IdAssetMap[id] = asset;
            link.SetTarget(asset);
            return link;
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