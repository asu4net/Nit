#include "ViewportPanel.h"
#include <Nit.h>
#include "ActorPanel.h"
#include "Editor.h"
#include "Scene/Components/EditorCameraComponent.h"
#include "Scene/Systems/EditorCameraSystem.h"

namespace Nit
{
    ViewportPanel::ViewportPanel(Editor* editor)
        : m_ViewportSize(1280, 720)
        , m_Editor(editor)
    {
        Renderer2D::GetInstance().SetRenderTarget(Renderer2D::RenderTarget::FrameBuffer);
    }

    void ViewportPanel::Draw()
    {
        DrawViewport();
    }

    void ViewportPanel::DrawViewport()
    {
        const Actor editorCamera = EditorCameraSystem::GetEditorCameraActor();
        
        if (!editorCamera.IsValid())
            return;
        
        auto& editorCameraComponent = editorCamera.Get<EditorCameraComponent>();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});

        ImGuiWindowClass windowClass;
        windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
        ImGui::SetNextWindowClass(&windowClass);
        
        ImGui::Begin("Viewport");
        
        const ImVec2 panelSize = ImGui::GetContentRegionAvail();
        
        if (const Vec2 viewportSize = {panelSize.x, panelSize.y }; viewportSize != m_ViewportSize )
        {
            m_ViewportSize = viewportSize;
            editorCameraComponent.CameraData.AspectRatio = m_ViewportSize.x / m_ViewportSize.y;
        }
        
        const ImTextureID fbId = reinterpret_cast<ImTextureID>(Renderer2D::GetInstance().GetFrameBuffer()->GetColorAttachment());
        ImGui::Image(fbId, {m_ViewportSize.x, m_ViewportSize.y}, {0, 1}, {1, 0});

        DrawGizmos();
        
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void ViewportPanel::DrawGizmos()
    {
        if (World::GetActiveScene().IsRuntimeEnabled() || !m_Editor)
            return;

        const Actor editorCameraActor = EditorCameraSystem::GetEditorCameraActor();

        if (!editorCameraActor.IsValid())
            return;

        const auto& camera = editorCameraActor.Get<EditorCameraComponent>();

        ImGuizmo::SetOrthographic(camera.CameraData.Projection == CameraProjection::Orthographic);
        ImGuizmo::SetDrawlist();
        const float windowWidth = ImGui::GetWindowWidth();
        const float windowHeight = ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

        const auto& cameraTransform = editorCameraActor.Get<TransformComponent>();
        
        Mat4 viewMatrix = MatIdentity;
        camera.CameraData.CalculateView(viewMatrix, cameraTransform.Position, cameraTransform.Rotation);
        Mat4 projectionMatrix = MatIdentity;
        camera.CameraData.CalculateProjection(projectionMatrix);
        
        //Gizmos
        const Actor selectedActor = m_Editor->GetActorPanel()->GetSelectedActor();
        if (selectedActor.IsValid())
        {
            if (!Input::IsKeyPressed(MOUSE_BUTTON_RIGHT))
            {
                if (Input::IsKeyPressed(KEY_W))
                    m_GizmoMode = GizmoMode::Translate;

                if (Input::IsKeyPressed(KEY_E))
                    m_GizmoMode = GizmoMode::Rotate;
            
                if (Input::IsKeyPressed(KEY_R))
                    m_GizmoMode = GizmoMode::Scale;
            }

            auto& selectedActorTransform = selectedActor.Get<TransformComponent>();
            
            Mat4 selectedActorModel = TransformStatics::GetModelMat4(selectedActorTransform);

            ImGuizmo::Manipulate(value_ptr(viewMatrix), value_ptr(projectionMatrix),
                       static_cast<ImGuizmo::OPERATION>(m_GizmoMode),
                       m_GizmosLocal ? ImGuizmo::LOCAL : ImGuizmo::WORLD,
                       glm::value_ptr(selectedActorModel),
                       nullptr,
                       m_Snap == 0.f ? nullptr : &m_Snap);
            
            if (ImGuizmo::IsUsing())
            {
                TransformStatics::SetFromMat4(selectedActorTransform, selectedActorModel);
            }
        }
    }
}