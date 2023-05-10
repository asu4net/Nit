#include "AudioManager.h"
#include <alc/alcmain.h>

namespace Nit
{
    void AudioManager::Start()
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

    void AudioManager::Finish()
    {
        alcDestroyContext(m_Context);
        alcCloseDevice(m_Device);
    }

    AudioSource AudioManager::CreateSource(const Shared<AudioBuffer>& audioBuffer)
    {
        uint32_t audioSourceHandler{0};
        alGenSources(1, &audioSourceHandler);
        alSourcei(audioSourceHandler, AL_BUFFER, audioBuffer->GetId());
        const AudioSource audioSource{audioSourceHandler};
        return audioSource;
    }

    void AudioManager::DestroySource(AudioSource& audioSource)
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

    void AudioManager::SetPitch(const AudioSource& audioSource, const uint32_t pitch)
    {
        alSourcei(audioSource.GetHandler(), AL_PITCH,
            static_cast<ALint>(pitch));
    }

    void AudioManager::SetGain(const AudioSource& audioSource, const uint32_t gain)
    {
        alSourcei(audioSource.GetHandler(), AL_GAIN,
            static_cast<ALint>(gain));
    }

    void AudioManager::SetLoop(const AudioSource& audioSource, const bool loop)
    {
        alSourcei(audioSource.GetHandler(), AL_LOOPING, loop);
    }

    void AudioManager::Translate(const AudioSource& audioSource, const glm::vec3& position)
    {
        alSource3f(audioSource.GetHandler(), AL_POSITION,
            position.x, position.y, position.z);
    }

    void AudioManager::SetVelocity(const AudioSource& audioSource, const glm::vec3& velocity)
    {
        alSource3f(audioSource.GetHandler(), AL_VELOCITY,
            velocity.x, velocity.y, velocity.z);
    }

    void AudioManager::Rotate(const AudioSource& audioSource, const glm::vec3& orientation)
    {
        alSource3f(audioSource.GetHandler(), AL_ORIENTATION,
            orientation.x, orientation.y, orientation.z);
    }

    void AudioManager::TranslateListener(const glm::vec3& position)
    {
        alListener3f(AL_POSITION, position.x, position.y, position.z);
    }

    void AudioManager::RotateListener(const glm::vec3& orientation)
    {
        alListener3f(AL_ORIENTATION, orientation.x, orientation.y, orientation.z);
    }

    void AudioManager::SetListenerVelocity(const glm::vec3& velocity)
    {
        alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }
}