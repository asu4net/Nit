#pragma once
#include "AssetRef.h"
#include "Asset.h"

namespace Nit
{
    class Animation : public Asset
    {
    public:
        struct Key
        {
            TString KeyRef;
            float KeyTime;
        };

        Animation() = default;
        
        void SetTime(float time) { m_Time = time; }
        float GetTime() const { return m_Time; }
        
        void PushKey(const Key& key);
        void PopKey(uint32_t index);
        void PushKeysFromSpriteSheet(const AssetRef spriteSheetRef, float keyTime = .1f);
        const Key& GetKey(uint32_t index) const;
        size_t GetSize() const;
        void AdjustTime();
        void Clear();

    private:
        TDynamicArray<Key> m_Keys;
        float m_Time = 0.f;
        void SortKeys();

        RTTR_ENABLE(Asset)
        RTTR_REGISTRATION_FRIEND
    };

    NIT_FORCE_LINK(Animation)
}