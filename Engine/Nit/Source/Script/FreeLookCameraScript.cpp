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

        const bool IsRightMousePressed = Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
        
        // Mouse pressed
        if (!m_bMouseDown && IsRightMousePressed)
        {
            m_bMouseDown = true;
            m_Offset = Math::ScreenToWorldPoint(camera.ProjectionMatrix * camera.ViewMatrix, Input::GetMousePosition(), Engine::GetScreenSize()) + m_AuxPosition;
        }
        
        // Mouse released
        if (m_bMouseDown && !IsRightMousePressed)
        {
            m_bMouseDown = false;
            m_AuxPosition = GetTransform().Position;
        }
                
        // Mouse hold
        if (IsRightMousePressed)
        {
            m_AuxCameraMatrix = Matrix4::OrthoProjection(Engine::GetWindow().GetAspect(), camera.Size, camera.NearPlane, camera.FarPlane)
                * Matrix4::ViewProjection(m_AuxPosition, Vector3::Zero);

            const Vector2 mouseWorld = Math::ScreenToWorldPoint(m_AuxCameraMatrix, Input::GetMousePosition(), Engine::GetScreenSize());

            GetTransform().Position = Vector3(mouseWorld * -1, GetTransform().Position.z) + Vector3(m_Offset.x, m_Offset.y, 0);
        }
    }
}