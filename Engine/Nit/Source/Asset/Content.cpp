#include "Content.h"
#include "Shader.h"

namespace Nit
{
    static const String AssetExtension = ".nitAsset";

    //TODO: Separar la deserialización de la llamada al load
    //TODO: Al deserializar un AssetRef siempre tienen que estar cargados todos los assets porque
    // se ha de resolver el retarget para el id del asset

    void Content::Init(const String& workingDirectory)
    {
        m_WorkingDirectory = workingDirectory;
        m_AssetDirectory = m_WorkingDirectory + "\\" + GetAssetsFolderName();
        NIT_LOG_TRACE("Content initialized!\n");
    }

    void Content::Finish()
    {
        UnloadAssets();
        NIT_LOG_TRACE("Content finished!\n");
    }

    void Content::SerializeAsset(const SharedPtr<Asset>& asset, const String& path)
    {
        AssetData assetData = asset->GetAssetData();
        assetData.Path = path;
        EnsureAssetDataConsistency(assetData, true);
        asset->SetAssetData(assetData);
        const String jsonNitAsset = Serialization::ToJson(asset);

        std::ofstream fileNitAssetProject("../" + path + "/" + assetData.Name + AssetExtension);
        std::ofstream fileNitAssetBinaries(GetWorkingDirectory() + "/" + path + "/" + assetData.Name + AssetExtension);
        
        fileNitAssetProject << jsonNitAsset;
        fileNitAssetBinaries << jsonNitAsset;
    }

    SharedPtr<Asset> Content::DeserializeAsset(const std::filesystem::path& assetPath)
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

        StringStream assetDataStream;
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
        SharedPtr<Asset> asset = variant.get_value<SharedPtr<Asset>>();
        return asset;
    }

    void Content::EachAsset(Delegate<void(const AssetRef&)> iterateFunc)
    {
        for (auto& [id, asset] : m_IdAssetMap)
        {
            iterateFunc(AssetRef(asset->GetAssetData().AssetId));
        }
    }

    WeakPtr<Asset> Content::GetAssetById(Id id)
    {
        if (!m_IdAssetMap.count(id))
            return WeakPtr<Asset>();

        return m_IdAssetMap[id];
    }

    AssetRef Content::GetAssetByName(const String& name)
    {
        if (!m_AssetNameIdMap.count(name))
            return {};

        const Id assetId = m_AssetNameIdMap[name];
        NIT_CHECK(m_IdAssetMap.count(assetId), "Internal error");
        return AssetRef(assetId);
    }

    void Content::LoadAssets()
    {
        UnloadAssets();

        NIT_LOG_TRACE("Loading assets...\n");
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(m_AssetDirectory))
        {
            const std::filesystem::path& dirPath = dirEntry.path();
            
            if (dirEntry.is_directory()) 
                continue;

            SharedPtr<Asset> asset = DeserializeAsset(dirPath);
            
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

    void Content::UnloadAssets()
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

    void Content::EnsureAssetDataConsistency(AssetData& assetData, bool bIgnoreChecks)
    {
        NIT_CHECK(!assetData.Name.empty(), "Asset must have a name!");

        if (!bIgnoreChecks)
        {
            NIT_CHECK(!m_AssetNameIdMap.count(assetData.Name), "An asset with the same Name already exists!");
            NIT_CHECK(!m_IdAssetMap.count(assetData.AssetId), "An asset with the same AssetId already exists!");
        }
        
        if (assetData.Path.empty())
        {
            assetData.Path = AssetData::s_EmptyPathString;
            assetData.AbsolutePath = AssetData::s_EmptyPathString;
        }
        else
        {
            assetData.AbsolutePath = m_WorkingDirectory + "\\" + assetData.Path;
        }
    }

    AssetRef Content::RegistryAsset(const SharedPtr<Asset>& asset, const AssetData& assetData)
    {
        AssetData finalAssetData = assetData;
        EnsureAssetDataConsistency(finalAssetData);
        finalAssetData.AssetType = asset->get_type().get_name().to_string();
        asset->SetAssetData(finalAssetData);
        m_IdAssetMap[finalAssetData.AssetId] = asset;
        m_AssetNameIdMap[finalAssetData.Name] = finalAssetData.AssetId;
        return AssetRef(finalAssetData.AssetId);
    }

    void Content::TryLoadAsset(AssetRef assetRef)
    {
        SharedPtr<Asset> asset = assetRef.GetPtr().lock();

        const AssetData assetData = asset->GetAssetData();
        const char* fileName = assetData.Name.c_str();

        if (asset->Load())
        {
            NIT_LOG_TRACE("loaded: %s\n", fileName);
            m_AssetCreatedEvent.Broadcast(assetRef);
        }
        else
        {
            NIT_LOG_WARN("Failed to load: %s", fileName);
            m_IdAssetMap.erase(assetData.AssetId);
        }
    }
}