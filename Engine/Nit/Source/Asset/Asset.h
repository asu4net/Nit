#pragma once

namespace Nit
{
    struct AssetData
    {
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
        Asset() = default;
        virtual ~Asset() {};
        
        virtual bool Load() { return true; };
        virtual void PostLoad() {};
        virtual void Unload() {};

        AssetData GetAssetData() const { return m_AssetData; };
        void SetAssetData(const AssetData& assetData) { m_AssetData = assetData; };
        
    private:        
        AssetData m_AssetData;

        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND
        friend struct ExtensionData;
    };

    NIT_FORCE_LINK(Asset)
}