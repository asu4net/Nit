#include "NitPCH.h"
#include "AssetManager.h"
#include "Core/Serialization.h"

namespace Nit
{
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
                asset->Initialize();
                m_IdAssetMap[asset->GetId()] = asset;
                m_NameIdMap[asset->GetName()] = asset->GetId();
            }
        }
    }

    void AssetManager::Finalize()
    {
        for (auto[id, asset] : m_IdAssetMap)
        {
            if (asset->GetPath() != "None")
            {
                const std::string jsonAssetInfo = Serialization::ToJson(asset);

                std::ofstream fileAssetInfo("../" + asset->GetPath() + ".assetInfo"); //TODO: Use display name
                std::ofstream fileAssetInfoExe(asset->GetPath() + ".assetInfo"); //TODO: Use display name
        
                fileAssetInfo << jsonAssetInfo;
                fileAssetInfoExe << jsonAssetInfo;
            }
            assert(asset->Unload());
        }
    }
}
