#pragma once
#include "Asset.h"
#include "AssetLink.h"

//TODO: Use path or name as key

namespace Nit
{
    class AssetManager : public Singleton<AssetManager>
    {
    public:
        template<typename T, typename ...TArgs>
        AssetLink<T> CreateAsset(const std::string& name, const std::string& path, TArgs&& ...args)
        {
            return CreateAssetWithId<T>(name, path, Id(), std::forward<TArgs>(args)...);
        }
        
        template<typename T, typename ...TArgs>
        AssetLink<T> CreateAssetWithId(const std::string& name, const std::string& path, const Id& id, TArgs&& ...args)
        {
            AssetLink<T> link;
            const rttr::type t = rttr::type::get<T>();
            
            if (!t.is_valid() || !t.is_derived_from<Asset>())
                return link;
            
            if (m_IdAssetMap.contains(id))
            {
                const Shared<T> asset = std::static_pointer_cast<T>(m_IdAssetMap[id]);
                link.SetTarget(asset);
                return link;
            }

            Shared<T> asset = CreateShared<T>(name, path, id, std::forward<TArgs>(args)...);
            if (!asset->Load())
                return {};
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