#include "NitPCH.h"
#include "AssetManager.h"

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

    void AssetManager::Initialize()
    {
    }

    void AssetManager::Finalize()
    {
        for (auto[id, asset] : m_IdAssetMap)
        {
            asset->Unload();
        }
    }

    Id AssetManager::SaveAsset(const std::string& path, const std::string& type, const std::string& name)
    {
        return 0;
    }
}