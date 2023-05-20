#pragma once
#include "AudioClip.h"
#include "AudioSource.h"

struct ALCdevice;
struct ALCcontext;

namespace Nit
{
    class AudioManager : public Singleton<AudioManager>
    {
    public:
        void Start();
        void Finish();

        AudioSource CreateSource(const Shared<AudioClip>& clip);

        void DestroySource(AudioSource& audioSource);
        void Play(const AudioSource& audioSource);
        void Stop(const AudioSource& audioSource);
        void SetPitch(const AudioSource& audioSource, uint32_t pitch);
        void SetGain(const AudioSource& audioSource, uint32_t gain);
        void SetLoop(const AudioSource& audioSource, bool loop);
        void Translate(const AudioSource& audioSource, const Vec3& position);
        void SetVelocity(const AudioSource& audioSource, const Vec3& velocity);
        void Rotate(const AudioSource& audioSource, const Vec3& orientation);
        void TranslateListener(const Vec3& position);
        void RotateListener(const Vec3& up, const Vec3& forward);
        void SetListenerVelocity(const Vec3& velocity);
        
    private:
        ALCcontext* m_Context;
        ALCdevice* m_Device;
        
        AudioManager() = default;
        friend class Singleton<AudioManager>;
    };
}