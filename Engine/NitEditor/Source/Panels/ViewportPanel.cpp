#include "ViewportPanel.h"
#include <Nit.h>

#include "Scene/Components/EditorCameraComponent.h"
#include "Scene/Systems/EditorCameraSystem.h"

namespace Nit
{
    ViewportPanel::ViewportPanel()
        : m_ViewportSize(1280, 720)
    {
        Renderer2D::GetInstance().SetRenderTarget(Renderer2D::RenderTarget::FrameBuffer);
    }

    void ViewportPanel::Draw()
    {
        const Actor editorCamera = EditorCameraSystem::GetEditorCameraActor();
        
        if (!editorCamera.IsValid())
            return;
        
        auto& editorCameraComponent = editorCamera.Get<EditorCameraComponent>();

        ImGui::Begin("Viewport");
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
        
        const ImVec2 panelSize = ImGui::GetContentRegionAvail();
        
        if (const Vec2 viewportSize = {panelSize.x, panelSize.y }; viewportSize != m_ViewportSize )
        {
            m_ViewportSize = viewportSize;
            editorCameraComponent.CameraData.AspectRatio = m_ViewportSize.x / m_ViewportSize.y;
        }
        
        const ImTextureID fbId = reinterpret_cast<ImTextureID>(Renderer2D::GetInstance().GetFrameBuffer()->GetId());
        ImGui::Image(fbId, {m_ViewportSize.x, m_ViewportSize.y}, {0, 1}, {1, 0});
        ImGui::PopStyleVar();
        ImGui::End();
    }
}
