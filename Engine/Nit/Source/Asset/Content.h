#pragma once
#include "Asset.h"
#include "AssetRef.h"

namespace Nit
{
    NIT_DECLARE_GLOBAL_EVENT_ONE_PARAM(AssetCreatedEvent, AssetRef, asset);
    NIT_DECLARE_GLOBAL_EVENT_ONE_PARAM(AssetDestroyedEvent, AssetRef, asset);
}

namespace Nit::Content
{
    void LoadAssets();
    void UnloadAssets();

    AssetCreatedEvent& OnAssetCreated();
    AssetDestroyedEvent& OnAssetDestroyed();

    void SerializeAsset(const SharedPtr<Asset>& asset, const String& path);
    SharedPtr<Asset> DeserializeAsset(const std::filesystem::path& assetPath);

    void GetAssetsOfType(const Type& type, DynamicArray<AssetRef>& assets);
    void EachAsset(Delegate<void(const AssetRef&)> iterateFunc);

    WeakPtr<Asset> GetAssetById(Id id);
    AssetRef GetAssetByName(const String& name);

    void EnsureAssetDataConsistency(AssetData& assetData, bool bIgnoreChecks = false);
    AssetRef RegistryAsset(const SharedPtr<Asset>& asset, const AssetData& assetData);
    bool TryLoadAsset(AssetRef assetLink);

    template<typename T, typename... TArgs>
    AssetRef CreateAsset(const AssetData& assetData, TArgs&&... args)
    {
        return RegistryAsset(CreateSharedPtr<T>(std::forward<TArgs>(args)...), assetData);
    }

    template<typename T, typename... TArgs>
    AssetRef CreateAsset(AssetData&& assetData, TArgs&&... args)
    {
        return RegistryAsset(CreateSharedPtr<T>(std::forward<TArgs>(args)...), assetData);
    }

    template<typename T, typename... TArgs>
    AssetRef CreateAsset(const String& assetName, TArgs&&... args)
    {
        return CreateAsset<T>(AssetData{ assetName }, std::forward<TArgs>(args)...);
    }
};