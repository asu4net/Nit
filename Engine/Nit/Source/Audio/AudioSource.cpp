#include "AudioSource.h"

namespace Nit
{
    AudioSource::AudioSource(const uint32_t& handler)
        : m_Handler(handler)
    {
    }

    void AudioSource::Reset()
    {
        m_Handler = 0;
    }
}