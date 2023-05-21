#include "AudioSourceComponent.h"
#include "Audio/AudioManager.h"

namespace Nit { NIT_FORCE_LINK_IMPL(AudioSourceComponent) }

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::class_<AudioSourceComponent>("AudioSourceComponent")
        .constructor<>()
        .property("Pitch", &AudioSourceComponent::Pitch)
        .property("Gain", &AudioSourceComponent::Gain)
        .property("Loop", &AudioSourceComponent::Loop)
        .property("AutoPlay", &AudioSourceComponent::AutoPlay)
        .property("Clip", &AudioSourceComponent::m_Clip);
}

namespace Nit
{
    void AudioStatics::RecreateSource(AudioSourceComponent& audioSource)
    {
        const AssetLink<AudioClip> clip = audioSource.m_Clip;
        AddClip(audioSource, clip);
    }

    void AudioStatics::AddClip(AudioSourceComponent& audioSource, const AssetLink<AudioClip>& clip)
    {
        RemoveClip(audioSource);
        
        if (clip.IsValid())
        {
            AudioManager& audioManager = AudioManager::GetInstance();
            audioSource.m_Clip = clip;
            audioSource.m_Source = audioManager.CreateSource(audioSource.m_Clip.Lock());
        }
    }

    void AudioStatics::RemoveClip(AudioSourceComponent& audioSource)
    {
        if (audioSource.m_Source.GetHandler() != 0)
        {
            AudioManager& audioManager = AudioManager::GetInstance();
            audioManager.DestroySource(audioSource.m_Source);
            audioSource.m_Source.Reset();
        }
        audioSource.m_Clip = AssetLink<AudioClip>();
    }

    void AudioStatics::Play(const AudioSourceComponent& audioSource)
    {
        if (audioSource.m_Source.GetHandler() != 0)
        {
            AudioManager& audioManager = AudioManager::GetInstance();
            audioManager.Play(audioSource.m_Source);
        }
    }

    void AudioStatics::Stop(const AudioSourceComponent& audioSource)
    {
        if (audioSource.m_Source.GetHandler() != 0)
        {
            AudioManager& audioManager = AudioManager::GetInstance();
            audioManager.Stop(audioSource.m_Source);
        }
    }
}