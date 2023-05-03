#include "AudioManager.h"
#include <alc/alcmain.h>

namespace Nit
{
    void AudioManager::Initialize()
    {
        m_Device = alcOpenDevice(nullptr);
        
        if (m_Device)
        {
            m_Context = alcCreateContext(m_Device,nullptr); 
            alcMakeContextCurrent(m_Context);
            return;
        }
    
        assert(false && "AudioManager Initialization failed");
    }

    void AudioManager::Finalize()
    {
        alcDestroyContext(m_Context);
        alcCloseDevice(m_Device);
    }

    AudioSource AudioManager::CreateAudioSource(const Shared<AudioBuffer>& audioBuffer)
    {
        uint32_t audioSourceHandler{0};
        alGenSources(1, &audioSourceHandler);
        alSourcei(audioSourceHandler, AL_BUFFER, audioBuffer->GetId());
        const AudioSource audioSource{audioSourceHandler};
        return audioSource;
    }

    void AudioManager::DestroyAudioSource(AudioSource& audioSource)
    {
        const uint32_t handler = audioSource.GetHandler();
        alDeleteSources(1, &handler);
        audioSource.Reset();
    }

    void AudioManager::Play(const AudioSource& audioSource)
    {
        alSourcePlay(audioSource.GetHandler());
    }

    void AudioManager::Stop(const AudioSource& audioSource)
    {
        alSourceStop(audioSource.GetHandler());
    }
}