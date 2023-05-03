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

        AudioSource CreateSource(const Shared<AudioBuffer>& audioBuffer);

        void DestroySource(AudioSource& audioSource);
        void Play(const AudioSource& audioSource);
        void Stop(const AudioSource& audioSource);
        void SetPitch(const AudioSource& audioSource, uint32_t pitch);
        void SetGain(const AudioSource& audioSource, uint32_t gain);
        void SetLoop(const AudioSource& audioSource, bool loop);
        void Translate(const AudioSource& audioSource, const glm::vec3& position);
        void SetVelocity(const AudioSource& audioSource, const glm::vec3& velocity);
        void Rotate(const AudioSource& audioSource, const glm::vec3& orientation);
        void TranslateListener(const glm::vec3& position);
        void RotateListener(const glm::vec3& orientation);
        void SetListenerVelocity(const glm::vec3& velocity);
        
    private:
        ALCcontext* m_Context;
        ALCdevice* m_Device;
        
        AudioManager() = default;
        friend class Singleton<AudioManager>;
    };
}