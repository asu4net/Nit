#include "NitPCH.h"
#include "AssetManager.h"
#include "Core/Serialization.h"

namespace Nit
{
    struct AssetInfo
    {
        Id Id{0};
        std::string Type;
        std::string Name;
        std::string Path; //Relative to the asset folder
    };
    
    RTTR_REGISTRATION
    {
        rttr::registration::class_<AssetInfo>("AssetInfo")
            .constructor<>()
            .property("Id",   &AssetInfo::Id)
            .property("Type", &AssetInfo::Type)
            .property("Name", &AssetInfo::Name)
            .property("Path", &AssetInfo::Path);
    }

    void AssetManager::SerializeAsset(const Shared<Asset>& asset)
    {
        const std::string jsonAssetInfo = Serialization::ToJson(asset);
        std::ofstream fileAssetInfo(asset->GetAbsolutePath() + ".assetInfo"); //TODO: Use display name
        fileAssetInfo << jsonAssetInfo;
    }

    void AssetManager::DeserializeAsset(const std::string& jsonAssetInfo)
    {
        
    }

    void AssetManager::Initialize()
    {
        static const std::string AssetDirectory = CurrentDirectory() + "\\Content";
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(AssetDirectory))
        {
            const std::filesystem::path& dirPath = dirEntry.path();
            
            if (!dirEntry.is_directory() && dirPath.filename().extension() == ".assetInfo")
            {
                const std::ifstream stream(dirPath);
                if (stream.fail()) continue;
                std::stringstream ss;
                ss << stream.rdbuf();

                // AssetInfo assetInfo;
                Asset assetData;
                Serialization::FromJson(ss.str(), assetData);
                
                rttr::type assetType = rttr::type::get_by_name(assetData.GetTypeName());
                
                if (!assetType.is_valid())
                    continue; ////TODO: destroy asset info file
                
                rttr::variant variant = assetType.create({assetData.GetName(), assetData.GetPath(), assetData.GetId()});

                if (!variant.is_valid())
                    continue; ////TODO: destroy asset info file
                
                rttr::instance assetInstance = variant;
                Serialization::FromJson(ss.str(), assetInstance);
                
                Shared<Asset> asset = variant.get_value<Shared<Asset>>();
                if (!asset->Load()) //TODO: destroy asset info file
                    continue;
                
                m_IdAssetMap[asset->GetId()] = asset;
            }
        }
    }

    void AssetManager::Finalize()
    {
        for (auto[id, asset] : m_IdAssetMap)
        {
            assert(asset->Unload());
        }
    }
}
