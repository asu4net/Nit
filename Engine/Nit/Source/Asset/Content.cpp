#include "Content.h"
#include "Shader.h"

namespace Nit::Content
{
    const TString AssetExtension = ".nitAsset";
    
    AssetCreatedEvent m_AssetCreatedEvent;
    AssetDestroyedEvent m_AssetDestroyedEvent;
    TMap<Id, TSharedPtr<Asset>> m_IdAssetMap;
    TMap<TString, Id> m_AssetNameIdMap;

    void SerializeAsset(const TSharedPtr<Asset>& asset, const TString& path)
    {
        AssetData assetData = asset->GetAssetData();
        EnsureAssetDataConsistency(assetData, true);
        asset->SetAssetData(assetData);
        const TString jsonNitAsset = Serialization::ToJson(asset);

        const TString assetStr = assetData.Name + AssetExtension;
        std::ofstream fileNitAssetBinaries(path.empty() ? assetStr : path + "\\" + assetStr);
        
        fileNitAssetBinaries << jsonNitAsset;
    }

    TSharedPtr<Asset> DeserializeAsset(const std::filesystem::path& assetPath)
    {
        if (assetPath.extension() != AssetExtension)
        {
            return nullptr;
        }

        const std::ifstream fileStream(assetPath);

        if (fileStream.fail())
        {
            return nullptr;
        }

        TStringStream assetDataStream;
        assetDataStream << fileStream.rdbuf();

        Asset assetData;
        Serialization::FromJson(assetDataStream.str(), assetData);

        Type assetType = rttr::type::get_by_name(assetData.GetAssetData().AssetType);

        if (!assetType.is_valid())
            return nullptr;

        Variant variant = assetType.create();
        
        if (!variant.is_valid())
            return nullptr;
        
        Instance instance = variant;
        Serialization::FromJson(assetDataStream.str(), instance);
        TSharedPtr<Asset> asset = variant.get_value<TSharedPtr<Asset>>();
        return asset;
    }

    void GetAssetsOfType(const Type& type, TDynamicArray<AssetRef>& assets)
    {
        for (auto& [id, asset] : m_IdAssetMap)
        {
            if (asset->GetAssetData().AssetType != type.get_name())
                continue;

            assets.emplace_back(AssetRef(asset->GetAssetData().AssetId));
        }
    }

    void EachAsset(Delegate<void(const AssetRef&)> iterateFunc)
    {
        for (auto& [id, asset] : m_IdAssetMap)
        {
            iterateFunc(AssetRef(asset->GetAssetData().AssetId));
        }
    }

    TWeakPtr<Asset> GetAssetById(Id id)
    {
        if (!m_IdAssetMap.count(id))
            return TWeakPtr<Asset>();

        return m_IdAssetMap[id];
    }

    AssetRef GetAssetByName(const TString& name)
    {
        if (!m_AssetNameIdMap.count(name))
            return {};

        const Id assetId = m_AssetNameIdMap[name];
        NIT_CHECK(m_IdAssetMap.count(assetId), "Internal error");
        return AssetRef(assetId);
    }

    bool HasAsset(const TString& name)
    {
        return m_AssetNameIdMap.count(name);
    }

    void LoadAssets()
    {
        UnloadAssets();

        NIT_LOG_TRACE("Loading assets...\n");
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(GetWorkingDirectory()))
        {
            const std::filesystem::path& dirPath = dirEntry.path();
            
            if (dirEntry.is_directory()) 
                continue;

            TSharedPtr<Asset> asset = DeserializeAsset(dirPath);
            
            if (!asset)
                continue;

            AssetRef assetRef = RegistryAsset(asset, asset->GetAssetData());
            TryLoadAsset(assetRef);
        }

        for (auto [id, asset] : m_IdAssetMap)
        {
            asset->PostLoad();
        }
    }

    void UnloadAssets()
    {
        if (m_IdAssetMap.empty())
            return;

        NIT_LOG_TRACE("Unloading assets...\n");
        for (auto [id, asset] : m_IdAssetMap)
        {
            asset->Unload();
        }

        m_IdAssetMap.clear();
        m_AssetNameIdMap.clear();
        NIT_LOG_TRACE("Assets unloaded!\n");
    }

    AssetCreatedEvent& OnAssetCreated()
    {
        return m_AssetCreatedEvent;
    }

    AssetDestroyedEvent& OnAssetDestroyed()
    {
        return m_AssetDestroyedEvent;
    }

    void EnsureAssetDataConsistency(AssetData& assetData, bool bIgnoreChecks)
    {
        NIT_CHECK(!assetData.Name.empty(), "Asset must have a name!");

        if (!bIgnoreChecks)
        {
            NIT_CHECK(!m_AssetNameIdMap.count(assetData.Name), "An asset with the same Name already exists!");
            NIT_CHECK(!m_IdAssetMap.count(assetData.AssetId), "An asset with the same AssetId already exists!");
        }
        
        assetData.AbsolutePath = GetWorkingDirectory().string() + "\\" + assetData.Path;
    }

    AssetRef RegistryAsset(const TSharedPtr<Asset>& asset, const AssetData& assetData)
    {
        AssetData finalAssetData = assetData;
        EnsureAssetDataConsistency(finalAssetData);
        finalAssetData.AssetType = asset->get_type().get_name().to_string();
        asset->SetAssetData(finalAssetData);
        m_IdAssetMap[finalAssetData.AssetId] = asset;
        m_AssetNameIdMap[finalAssetData.Name] = finalAssetData.AssetId;
        return AssetRef(finalAssetData.AssetId);
    }

    bool TryLoadAsset(AssetRef assetRef)
    {
        TSharedPtr<Asset> asset = assetRef.GetWeak().lock();

        const AssetData assetData = asset->GetAssetData();
        const char* fileName = assetData.Name.c_str();

        if (asset->Load())
        {
            NIT_LOG_TRACE("loaded: %s\n", fileName);
            m_AssetCreatedEvent.Broadcast(assetRef);
            return true;
        }
        else
        {
            NIT_LOG_WARN("Failed to load: %s", fileName);
            m_IdAssetMap.erase(assetData.AssetId);
            asset.reset();
            return false;
        }
    }
}