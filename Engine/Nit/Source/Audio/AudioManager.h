#pragma once
#include "AudioBuffer.h"
#include "AudioSource.h"

struct ALCdevice;
struct ALCcontext;

namespace Nit
{
    class AudioManager : public Singleton<AudioManager>
    {
    public:
        void Initialize();
        void Finalize();

        AudioSource CreateAudioSource(const Shared<AudioBuffer>& audioBuffer);
        void DestroyAudioSource(AudioSource& audioSource);
        void Play(const AudioSource& audioSource);
        void Stop(const AudioSource& audioSource);
        
    private:
        ALCcontext* m_Context;
        ALCdevice* m_Device;
        
        AudioManager() = default;
        friend class Singleton<AudioManager>;
    };
}