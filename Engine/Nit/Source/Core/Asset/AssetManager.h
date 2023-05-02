#pragma once
#include "Asset.h"
#include "AssetLink.h"

//TODO: Use path or name as key

namespace Nit
{
    class AssetManager : public Singleton<AssetManager>
    {
    public:
        template<typename T>
        AssetLink<T> CreateAsset(const std::string& name, const std::string& path, const Id& id = Id())
        {
            AssetLink<T> link;
            const rttr::type t = rttr::type::get<T>();
            
            if (!t.is_valid() || !t.is_derived_from<Asset>())
                return link;
            
            if (m_IdAssetMap.contains(id) || m_NameIdMap.contains(name))
            {
                const Shared<T> asset = std::static_pointer_cast<T>(m_IdAssetMap[m_NameIdMap[name]]);
                link.SetTarget(asset);
                return link;
            }

            Shared<T> asset = CreateShared<T>(name, path, id);
            if (!asset->Load())
                return {};

            asset->SetTypeName(std::string(asset->get_type().get_name()));
            m_IdAssetMap[id] = asset;
            m_NameIdMap[name] = id;
            link.SetTarget(asset);

            if (asset->GetPath() != "None")
                SerializeAsset(m_IdAssetMap[id]);
            
            return link;
        }
        
        void Initialize();
        void Finalize();
    
    private:
        std::unordered_map<Id, Shared<Asset>> m_IdAssetMap;
        std::unordered_map<std::string, Id> m_NameIdMap;
        //TODO: Add name || path /id map
        
        AssetManager() = default;

        void SerializeAsset(const Shared<Asset>& asset);
        void DeserializeAsset(const std::string& jsonAssetInfo);
        
        friend class Singleton<AssetManager>;
    };
}