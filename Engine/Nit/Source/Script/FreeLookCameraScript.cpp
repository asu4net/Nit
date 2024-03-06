#include "FreeLookCameraScript.h"
#include "Input\Input.h"
#include "Input\KeyCodes.h"
#include "Component\CameraComponent.h"
#include "Component\TransformComponent.h"
#include "Window\Window.h"
#include "Core\Engine.h"

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<FreeLookCameraScript>("FreeLookCameraScript")
        .constructor<>();
}

namespace Nit
{
    NIT_FORCE_LINK_IMPL(FreeLookCameraScript);

    void FreeLookCameraScript::OnStart()
    {
        auto& camera = Get<CameraComponent>();
        m_DesiredZoom = camera.Size;
        
        Engine::GetWindow().Events().ScrollEvent.Add([&](const Vector2& offset) {

            if (!Input::IsEnabled()) return;
            
            m_DesiredZoom -= offset.y * ZoomStep;
            m_DesiredZoom = std::clamp(m_DesiredZoom, camera.NearPlane, Math::FloatMax());
        });
    }

    void FreeLookCameraScript::OnUpdate()
    {
        UpdateCameraZoom();
        UpdateCameraPosition();
    }

    void FreeLookCameraScript::UpdateCameraZoom()
    {
        auto& camera = Get<CameraComponent>();

        if (std::abs(m_DesiredZoom - camera.Size) > .01f)
        {
            const float dir = camera.Size < m_DesiredZoom ? 1.f : -1.f;
            camera.Size += ZoomSpeed * dir * Time::GetDeltaTime();
        }
        else
        {
            camera.Size = m_DesiredZoom;
        }
    }

    void FreeLookCameraScript::UpdateCameraPosition()
    {
        if (!CanMove)
            return;

        CameraComponent& camera = Get<CameraComponent>();

        
    }
}