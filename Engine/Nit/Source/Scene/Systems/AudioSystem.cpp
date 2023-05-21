#include "AudioSystem.h"
#include "CameraSystem.h"
#include "Audio/AudioManager.h"
#include "Scene/Components/AudioSourceComponent.h"
#include "Scene/Components/TransformComponent.h"

namespace Nit { NIT_FORCE_LINK_IMPL(AudioSystem) }

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::class_<AudioSystem>("AudioSystem")
        .constructor<>()
        .constructor<const Weak<Scene>&>();
}

namespace Nit
{
    AudioSystem::AudioSystem(const Weak<Scene>& scene)
        : SceneSystem(scene)
    {
        Registry().on_construct<AudioSourceComponent>().connect<&AudioSystem::OnAudioSourceComponentAdded>(this);
        Registry().on_destroy<AudioSourceComponent>().connect<&AudioSystem::OnAudioSourceComponentRemoved>(this);
    }

    void AudioSystem::OnAudioSourceComponentAdded(entt::registry&, const entt::entity entity)
    {
        const Actor actor{entity, RegistryPtr()};
        auto& audioSource = actor.Get<AudioSourceComponent>();
        AudioStatics::RecreateSource(audioSource);
    }

    void AudioSystem::OnAudioSourceComponentRemoved(entt::registry&, const entt::entity entity)
    {
        const Actor actor{entity, RegistryPtr()};
        auto& audioSource = actor.Get<AudioSourceComponent>();
        AudioStatics::RemoveClip(audioSource);
    }

    void AudioSystem::OnUpdate(const TimeStep& timeStep)
    {
        AudioManager& audioManager = AudioManager::GetInstance();

        // Update configuration
        const Actor cameraActor = CameraSystem::GetMainCameraActor();
        
        if (cameraActor.IsValid())
        {
            const auto& cameraTransform = cameraActor.Get<TransformComponent>();
            audioManager.TranslateListener(cameraTransform.Position);
            audioManager.RotateListener(TransformStatics::GetUpVec3(cameraTransform),
                TransformStatics::GetForwardVec3(cameraTransform));
        }
        
        const auto view = Registry().view<AudioSourceComponent, TransformComponent>();
        view.each([&](const AudioSourceComponent& audioSource, const TransformComponent&)
        {
            const AudioSource& source = audioSource.m_Source;
            
            if (source.GetHandler() == 0) return;
            
            audioManager.SetPitch(source, audioSource.Pitch);
            audioManager.SetGain(source, audioSource.Gain);
            audioManager.SetLoop(source, audioSource.Loop);
            //audioManager.SetVelocity(source, audioSource.Velocity); TODO: Calculate velocity
        });
    }
}