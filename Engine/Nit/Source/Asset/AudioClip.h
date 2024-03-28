#pragma once
#include "Asset.h"

namespace Nit
{
    class AudioClip : public Asset
    {
    public:
        AudioClip() = default;

        uint32_t GetId()     const { return m_BufferId; }
        uint32_t GetFormat() const { return m_Format; }
        char*    GetData()   const { return m_Data; }
        uint32_t GetSize()   const { return m_Size; }
        uint32_t GetFrec()   const { return m_Frec; }
        
        bool Load()   override;
        void Unload() override;

    private:
        uint32_t m_BufferId = 0;
        uint32_t m_Format   = 0;
        char*    m_Data     = nullptr;
        uint32_t m_Size     = 0;
        uint32_t m_Frec     = 0;
        
        RTTR_ENABLE(Asset)
        RTTR_REGISTRATION_FRIEND
    };

    NIT_FORCE_LINK(AudioClip)
}
