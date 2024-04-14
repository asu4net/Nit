#pragma once

namespace Nit
{
    struct AssetData
    {
        TString Name;
        TString Path;
        TString AbsolutePath;
        Id AssetId;
        TString AssetType;
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