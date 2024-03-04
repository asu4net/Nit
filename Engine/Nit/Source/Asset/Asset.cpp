#include "Asset.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(AssetData);
    NIT_FORCE_LINK_IMPL(Asset);

    Type Asset::GetAssetTypeFromExtension(const String& extension)
    {
        NIT_CHECK(s_ExtensionAssetTypeMap.count(extension), "Extension not registered!");
        Type assetClass = Type::get_by_name(s_ExtensionAssetTypeMap[extension]);
        return assetClass;
    }

    ExtensionData::ExtensionData(const String& assetType, const String& extension)
    {
        Type assetClass = Type::get_by_name(assetType);
        NIT_CHECK(assetClass.is_valid() && assetClass.is_derived_from(Type::get<Asset>()), "Type should be derived from asset!");
        Asset::s_ExtensionAssetTypeMap[extension] = assetType;
    }

}

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<AssetData>("AssetData")
        .constructor<>()
        .property("Name", &AssetData::Name)
        .property("Path", &AssetData::Path)
        .property("AssetId", &AssetData::AssetId)
        .property("AssetType", &AssetData::AssetType);

    rttr::registration::class_<Asset>("Asset")
        .constructor<>()
        .property("AssetData", &Asset::m_AssetData);
}