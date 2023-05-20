#pragma once
#include "ActorComponent.h"
#include "Audio/AudioClip.h"
#include "Audio/AudioSource.h"
#include "Core/Asset/AssetLink.h"

namespace Nit
{
    struct AudioSourceComponent : ActorComponent
    {
        AudioSourceComponent() = default;
        AudioSourceComponent(const AssetLink<AudioClip>& clip)
            : m_Clip(clip)
        {}
        
        uint32_t Pitch = 1;
        uint32_t Gain = 1;
        bool Loop = false;
        bool AutoPlay = false;
        
    private:
        AssetLink<AudioClip> m_Clip;
        AudioSource m_Source;
        
        RTTR_ENABLE(ActorComponent)
        RTTR_REGISTRATION_FRIEND
        
        friend struct AudioStatics;
        friend class AudioSystem;
    };
    NIT_FORCE_LINK(AudioSourceComponent)
    
    struct AudioStatics
    {
        static void RecreateSource(AudioSourceComponent& audioSource);
        static void AddClip(AudioSourceComponent& audioSource, const AssetLink<AudioClip>& clip);
        static void RemoveClip(AudioSourceComponent& audioSource);
        static void Play(const AudioSourceComponent& audioSource);
        static void Stop(const AudioSourceComponent& audioSource);
    };
}