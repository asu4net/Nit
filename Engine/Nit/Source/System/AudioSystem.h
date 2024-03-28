#pragma once

namespace Nit
{
    class AudioSource
    {
    public:
        AudioSource() = default;
        AudioSource(const uint32_t& handler) : m_Handler(handler) {}

        uint32_t GetHandler() const { return m_Handler; }
        void Reset() { m_Handler = 0; }
        
    private:
        uint32_t m_Handler = 0;
    };
    
    class AudioClip;

    namespace AudioSystem
    {
        // *** UTILITY FUNCTIONS ***
        
        AudioSource CreateSource(const SharedPtr<AudioClip>& clip);
        void DestroySource(AudioSource& audioSource);
        void Play(const AudioSource& audioSource);
        void Stop(const AudioSource& audioSource);
        bool IsPlaying(const AudioSource& audioSource);
        void SetPitch(const AudioSource& audioSource, uint32_t pitch);
        void SetGain(const AudioSource& audioSource, uint32_t gain);
        void SetLoop(const AudioSource& audioSource, bool loop);
        void Translate(const AudioSource& audioSource, const Vector3& position);
        void SetVelocity(const AudioSource& audioSource, const Vector3& velocity);
        void Rotate(const AudioSource& audioSource, const Vector3& orientation);
        void TranslateListener(const Vector3& position);
        void RotateListener(const Vector3& up, const Vector3& forward);
        void SetListenerVelocity(const Vector3& velocity); 
        
        // *** UTILITY FUNCTIONS END ***
        
        void Register();
    }
}