#include "TextSystem.h"

#include "Core/Asset/AssetManager.h"
#include "Scene/Components/TextComponent.h"
#include "Scene/Components/TransformComponent.h"

RTTR_REGISTRATION
{
    using namespace Nit;
    using namespace rttr;

    registration::class_<TextSystem>("TextSystem")
        .constructor<>()
        .constructor<const Weak<Scene>&>();
}

namespace Nit
{
    TextSystem::TextSystem(const Weak<Scene>& scene)
        : SceneSystem(scene)
    {
        Registry().on_construct<TextComponent>().connect<&TextSystem::OnTextComponentAdded>(this);
    }

    void TextSystem::OnStart()
    {
        AssetManager& assetManager = AssetManager::GetInstance();
        m_DefaultFont = assetManager.GetAssetByName<Font>("CascadiaMono");
    }

    void TextSystem::OnUpdate(const TimeStep& timeStep)
    {
        const auto view = Registry().view<TransformComponent, TextComponent>();

        view.each([&](const TransformComponent& transform, const TextComponent& text)
        {
            GetScene().GetSceneRenderer().PushTextQuad({
                text.Text,
                TransformStatics::GetModelMat4(transform),
                text.Color,
                text.Font.IsValid() ? text.Font.Lock() : nullptr,
                text.Size,
                text.Spacing
            });
        });
    }

    void TextSystem::OnTextComponentAdded(entt::registry&, const entt::entity entity)
    {
        const Actor actor{entity, RegistryPtr()};
        auto& text = actor.Get<TextComponent>();
        if (!text.Font.IsValid())
            text.Font = m_DefaultFont;
    }
}
