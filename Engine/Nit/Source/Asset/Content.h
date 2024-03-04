#pragma once
#include "Asset.h"
#include "AssetRef.h"

namespace Nit
{
    NIT_DECLARE_EVENT_ONE_PARAM(class Content, AssetCreatedEvent, AssetRef, asset);
    NIT_DECLARE_EVENT_ONE_PARAM(class Content, AssetDestroyedEvent, AssetRef, asset);

    class Content
    {
    public:
        static void Init(const String& workingDirectory);
        static void LoadAssets();
        static void UnloadAssets();
        static void Finish();

        static AssetCreatedEvent& OnAssetCreated() { return m_AssetCreatedEvent; }
        static AssetDestroyedEvent& OnAssetDestroyed() { return m_AssetDestroyedEvent; }

        static void SerializeAsset(const SharedPtr<Asset>& asset, const String& path);
        static SharedPtr<Asset> DeserializeAsset(const std::filesystem::path& assetPath);

        // Temporal
        template<typename T>
        static void GetAssetsOfType(DynamicArray<AssetRef>& assets)
        {
            for (auto& [id, asset] : m_IdAssetMap)
            {
                SharedPtr<T> desiredAsset = std::dynamic_pointer_cast<T>(asset);
                if (!desiredAsset)
                    continue;
                assets.emplace_back(AssetRef(desiredAsset->GetAssetData().AssetId));
            }
        }

        static void GetAssetsOfType(const Type& type, DynamicArray<AssetRef>& assets)
        {
            for (auto& [id, asset] : m_IdAssetMap)
            {
                if (asset->GetAssetData().AssetType != type.get_name())
                    continue;

                assets.emplace_back(AssetRef(asset->GetAssetData().AssetId));
            }
        }

        static void EachAsset(Delegate<void(const AssetRef&)> iterateFunc);

        static WeakPtr<Asset> GetAssetById(Id id);
        static AssetRef GetAssetByName(const String& name);

        template<typename T, typename... TArgs>
        static AssetRef CreateAsset(AssetData&& assetData, TArgs&&... args)
        {
            return RegistryAsset(CreateSharedPtr<T>(std::forward<TArgs>(args)...), assetData);
        }

        template<typename T, typename... TArgs>
        static AssetRef CreateAsset(const String& assetName, TArgs&&... args)
        {
            return CreateAsset<T>(AssetData{ assetName }, std::forward<TArgs>(args)...);
        }

    private:
        static void EnsureAssetDataConsistency(AssetData& assetData, bool bIgnoreChecks = false);
        static AssetRef RegistryAsset(const SharedPtr<Asset>& asset, const AssetData& assetData);
        static void TryLoadAsset(AssetRef assetLink);

        inline static AssetCreatedEvent m_AssetCreatedEvent;
        inline static AssetDestroyedEvent m_AssetDestroyedEvent;

        inline static String m_WorkingDirectory;
        inline static String m_AssetDirectory;
        
        inline static Map<Id, SharedPtr<Asset>> m_IdAssetMap;
        inline static Map<String, Id> m_AssetNameIdMap;
    };
}