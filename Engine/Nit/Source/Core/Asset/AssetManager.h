#pragma once
#include "Asset.h"
#include "AssetLink.h"

namespace Nit
{
    class AssetManager : public Singleton<AssetManager>
    {
    public:
        Weak<Asset> GetAssetById(const Id& id);
        
        template<typename T>
        void GetAssetsOfType(std::vector<AssetLink>& assetLinks)
        {
            for (auto[id, asset] : m_IdAssetMap)
            {
                if (asset->GetTypeName() != rttr::type::get<T>().get_name()) continue;
                assetLinks.push_back({asset->GetName(), asset->GetId(), asset->GetTypeName()});
            }
        }
        
        template<typename T>
        AssetLink CreateAsset(const std::string& name, const std::string& path, const Id& id = Id())
        {
            const rttr::type t = rttr::type::get<T>();
            
            if (!t.is_valid() || !t.is_derived_from<Asset>())
                return {};

            AssetLink link;
            
            if (m_IdAssetMap.contains(id) || m_NameIdMap.contains(name))
            {
                const Shared<T> asset = std::static_pointer_cast<T>(m_IdAssetMap[m_NameIdMap[name]]);
                link = {name, id, t.get_name().to_string()};
                return link;
            }
            
            Shared<T> asset = CreateShared<T>(name, path, id);
            if (!asset->Load())
                return {};
            
            asset->SetTypeName(std::string(asset->get_type().get_name()));
            m_IdAssetMap[id] = asset;
            m_NameIdMap[name] = id;
            link = {name, id, t.get_name().to_string()};
            
            return link;
        }
        
        bool ImportAsset(const std::filesystem::path& path);
        AssetLink GetAssetByName(const std::string& assetName);
        
        void Start();
        void Finish();
    
    private:
        std::unordered_map<Id, Shared<Asset>> m_IdAssetMap;
        std::unordered_map<std::string, Id> m_NameIdMap;
        
        AssetManager() = default;
        bool SerializeAsset(const Shared<Asset>& asset);
        bool DeserializeAsset(const std::filesystem::path& assetInfoPath);
        friend class Singleton<AssetManager>;
    };
}