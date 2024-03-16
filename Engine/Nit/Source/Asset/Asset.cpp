#include "Asset.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(AssetData);
    NIT_FORCE_LINK_IMPL(Asset);
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