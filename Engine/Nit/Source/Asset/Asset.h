#pragma once

namespace Nit
{
    struct AssetData
    {
        inline static const char* s_EmptyPathString = "NO_PATH";

        String Name;
        String Path;
        String AbsolutePath;
        Id AssetId;
        String AssetType;
    };
    
    NIT_FORCE_LINK(AssetData)

    class Asset
    {
    public:
        static Type GetAssetTypeFromExtension(const String& extension);

        Asset() = default;
        virtual ~Asset() {};
        
        virtual bool Load() { return true; };
        virtual void PostLoad() {};
        virtual void Unload() {};

        AssetData GetAssetData() const { return m_AssetData; };
        void SetAssetData(const AssetData& assetData) { m_AssetData = assetData; };
        
    private:
        inline static Map<String, String> s_ExtensionAssetTypeMap;
        
        AssetData m_AssetData;

        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND
        friend struct ExtensionData;
    };

    NIT_FORCE_LINK(Asset)
    
    struct ExtensionData
    {
        ExtensionData(const String& assetType, const String& extension);
    };

#define YDECLARE_ASSET_EXTENSION(_ASSET_TYPE, _EXTENSION) \
    inline static ExtensionData _ASSET_TYPE##_EXTENSION = ExtensionData(#_ASSET_TYPE, #_EXTENSION);
}