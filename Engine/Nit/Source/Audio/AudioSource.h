#pragma once

namespace Nit
{
    class AudioSource
    {
    public:
        AudioSource() = default;
        AudioSource(const uint32_t& handler);
        uint32_t GetHandler() const { return m_Handler; }
        void Reset();
        
    private:
        uint32_t m_Handler{0};
    };
}