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
        Engine::GetInstance().SetUseWindowScreenSize(false);
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

        if (Input::IsConsumedByEditor() && ImGui::IsWindowFocused())
            Input::SetConsumedByEditor(false);
        
        ImGui::Checkbox("Local", &m_GizmosLocal);
        ImGui::SameLine();
        ImGui::Checkbox("Use snap", &m_bGizmosSnapEnabled);
        ImGui::SameLine();
        ImGui::PushItemWidth(50.f);
        ImGui::DragFloat("Snap", &m_GizmosSnap, 0.05f, 0, 0, "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::Checkbox("3D", &m_3D);
        
        const ImVec2 panelSize = ImGui::GetContentRegionAvail();
        
        if (const Vec2 viewportSize = {panelSize.x, panelSize.y }; viewportSize != m_ViewportSize )
        {
            m_ViewportSize = viewportSize;
            Engine::GetInstance().SetScreenWidth(static_cast<uint32_t>(m_ViewportSize.x));
            Engine::GetInstance().SetScreenHeight(static_cast<uint32_t>(m_ViewportSize.y));
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

        auto& camera = editorCameraActor.Get<EditorCameraComponent>();

        if (m_3D)
            camera.CameraData.Projection = CameraProjection::Perspective;
        else
            camera.CameraData.Projection = CameraProjection::Orthographic;
        
        ImGuizmo::SetOrthographic(!m_3D);
        ImGuizmo::SetDrawlist();
        const float windowWidth = ImGui::GetWindowWidth();
        const float windowHeight = ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

        const auto& cameraTransform = editorCameraActor.Get<TransformComponent>();
        
        Mat4 viewMatrix = MatIdentity;
        camera.CameraData.CalculateView(viewMatrix, cameraTransform.Position, Quat(cameraTransform.Rotation));
        Mat4 projectionMatrix = MatIdentity;
        camera.CameraData.CalculateProjection(projectionMatrix);
        
        //Gizmos
        const Actor selectedActor = m_Editor->GetActorPanel()->GetSelectedActor();
        
        if (selectedActor.IsValid())
        {
            if (selectedActor.Has<EditorCameraComponent>())
                return;
            
            if (!Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
                !Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
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
            
            float snap[3] = { m_GizmosSnap, m_GizmosSnap, m_GizmosSnap };
            
            ImGuizmo::Manipulate(value_ptr(viewMatrix), value_ptr(projectionMatrix),
                       static_cast<ImGuizmo::OPERATION>(m_GizmoMode),
                       m_GizmosLocal ? ImGuizmo::LOCAL : ImGuizmo::WORLD,
                       glm::value_ptr(selectedActorModel),
                       nullptr,
                       m_bGizmosSnapEnabled ? snap : nullptr);
            
            if (ImGuizmo::IsUsing())
            {
                Vec3 position, rotation, scale;
                Math::Decompose(selectedActorModel, position, rotation, scale);

                const glm::vec3 deltaRotation = rotation - selectedActorTransform.Rotation;
                selectedActorTransform.Position = m_3D ? position : Vec3(position.x, position.y, selectedActorTransform.Position.z);
                selectedActorTransform.Rotation += deltaRotation;
                selectedActorTransform.Scale = scale;
            }
        }
    }
}