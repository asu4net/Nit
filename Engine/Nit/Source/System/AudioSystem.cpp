#include "AudioSystem.h"
#include <AL/alc.h>
#include <alc/alcmain.h>
#include "Core/Engine.h"
#include "Asset/AudioClip.h"

namespace Nit::AudioSystem
{
    ALCcontext* OpenALContext = nullptr;
    ALCdevice*  OpenALDevice  = nullptr;
    
    // *** UTILITY FUNCTIONS ***

    AudioSource CreateSource(const TSharedPtr<AudioClip>& clip)
    {
        uint32_t audioSourceHandler = 0;
        alGenSources(1, &audioSourceHandler);
        alSourcei(audioSourceHandler, AL_BUFFER, clip->GetId());
        const AudioSource audioSource{audioSourceHandler};
        return audioSource;
    }

    void DestroySource(AudioSource& audioSource)
    {
        const uint32_t handler = audioSource.GetHandler();
        alDeleteSources(1, &handler);
        audioSource.Reset();
    }

    void Play(const AudioSource& audioSource)
    {
        alSourcePlay(audioSource.GetHandler());
    }

    void Stop(const AudioSource& audioSource)
    {
        alSourceStop(audioSource.GetHandler());
    }

    bool IsPlaying(const AudioSource& audioSource)
    {
        ALint state;
        alGetSourcei(audioSource.GetHandler(), AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }

    void SetPitch(const AudioSource& audioSource, const uint32_t pitch)
    {
        alSourcei(audioSource.GetHandler(), AL_PITCH,
            static_cast<ALint>(pitch));
    }

    void SetGain(const AudioSource& audioSource, const uint32_t gain)
    {
        alSourcei(audioSource.GetHandler(), AL_GAIN,
            static_cast<ALint>(gain));
    }

    void SetLoop(const AudioSource& audioSource, const bool loop)
    {
        alSourcei(audioSource.GetHandler(), AL_LOOPING, loop);
    }

    void Translate(const AudioSource& audioSource, const CVector3& position)
    {
        alSource3f(audioSource.GetHandler(), AL_POSITION,
            position.x, position.y, position.z);
    }

    void SetVelocity(const AudioSource& audioSource, const CVector3& velocity)
    {
        alSource3f(audioSource.GetHandler(), AL_VELOCITY,
            velocity.x, velocity.y, velocity.z);
    }

    void Rotate(const AudioSource& audioSource, const CVector3& orientation)
    {
        alSource3f(audioSource.GetHandler(), AL_ORIENTATION,
            orientation.x, orientation.y, orientation.z);
    }

    void TranslateListener(const CVector3& position)
    {
        alListener3f(AL_POSITION, position.x, position.y, position.z);
    }
  
    void RotateListener(const CVector3& up, const CVector3& forward)
    {
        const float values[] = {up.x, up.y, up.z, forward.x, forward.y, forward.z};
        alListenerfv(AL_ORIENTATION, values);
    }

    void SetListenerVelocity(const CVector3& velocity)
    {
        alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }

    // *** UTILITY FUNCTIONS END ***    
    
    void OnCreate();
    void OnDestroy();

    void Register()
    {
        Engine::CreateSystem("AudioSystem", AudioExecutionOrder, ExecutionContext::Runtime);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::Destroy, &OnDestroy);
    }

    void OnCreate()
    {
        OpenALDevice = alcOpenDevice(nullptr);

        if (OpenALDevice)
        {
            OpenALContext = alcCreateContext(OpenALDevice, nullptr); 
            alcMakeContextCurrent(OpenALContext);
            return;
        }
    
        NIT_CHECK(false, "AudioSystem Initialization failed!\n");
    }

    void OnDestroy()
    {
        alcDestroyContext(OpenALContext);
        alcCloseDevice(OpenALDevice);
    }
}
