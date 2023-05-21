#include "AssetLink.h"

#include "AssetManager.h"
#include "Rendering/Data/Texture2D.h"

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Nit;

    registration::class_<AssetLink>("AssetLink")
        .property("Name", &AssetLink::m_Name)
        .property("Id", &AssetLink::m_Id)
        .property("Type", &AssetLink::m_TypeName);
}

namespace Nit
{
    AssetLink::AssetLink(const std::string& name, const Id& id, const std::string& typeName)
        : m_Name(name)
        , m_Id(id)
        , m_TypeName(typeName)
    {}
    
    Shared<Asset> AssetLink::Get() const
    {
        AssetManager& assetManager = AssetManager::GetInstance();
        const Weak<Asset> asset = assetManager.GetAssetById(m_Id);
        if (asset.expired()) return nullptr;
        return asset.lock();
    }
}