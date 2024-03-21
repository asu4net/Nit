#include "ViewportWindow.h"
#include <imgui.h>
#include "Core/Engine.h"
#include "Render/Renderer.h"
#include "Render/Framebuffer.h"
#include "Entity/Entity.h"
#include "System/CameraSystem.h"
#include "Component/NameComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TransformComponent.h"
#include "ImGuizmo.h"
#include "EditorSystem.h"

namespace Nit::ViewportWindow
{
    Vector2 ViewportSize;
    Vector2 ViewportMinBound;
    Vector2 ViewportMaxBound;
    Vector2 MousePosition;

    Vector2 GetMousePosition()
    {
        return MousePosition;
    }

    void Show(bool* bShow, Framebuffer* targetFramebuffer)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

        ImGui::Begin("Viewport", bShow);
        targetFramebuffer->Bind();

        if (!Input::IsEnabled() && ImGui::IsWindowFocused())
            Input::SetEnabled(true);
        
        const ImVec2 viewportOffset = ImGui::GetCursorPos();
        const ImVec2 panelSize = ImGui::GetContentRegionAvail();

        if (const Vector2 viewportSize = { panelSize.x, panelSize.y }; viewportSize != ViewportSize)
        {
            ViewportSize = viewportSize;
            Engine::SetScreenSize(ViewportSize);
            targetFramebuffer->Resize(Engine::GetScreenWidth(), Engine::GetScreenHeight());
            targetFramebuffer->Bind();
        }

        const ImTextureID fbId = reinterpret_cast<ImTextureID>(targetFramebuffer->GetColorAttachmentRendererID());
        ImGui::Image(fbId, { ViewportSize.x, ViewportSize.y }, { 0, 1 }, { 1, 0 });

        ImVec2 windowSize = ImGui::GetWindowSize();

        ImVec2 minBound = ImGui::GetWindowPos();
        minBound.x += viewportOffset.x;
        minBound.y += viewportOffset.y;

        ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };

        ViewportMinBound = { minBound.x, minBound.y };
        ViewportMaxBound = { maxBound.x, maxBound.y };

        auto [mx, my] = ImGui::GetMousePos();
        mx -= ViewportMinBound.x;
        my -= ViewportMinBound.y;
        my = ViewportSize.y - my;

        const int mouseX = (int)mx;
        const int mouseY = (int)my;

        MousePosition = { (float)mouseX, (float)mouseY };

        //Entity selection 
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsOver())
        {
            if (mouseX >= 0 && mouseY >= 0 && mouseX < ViewportSize.x && mouseY < ViewportSize.y)
            {
                int entityID = targetFramebuffer->ReadPixel(1, mouseX, mouseY);
                RawEntity rawEntity = (RawEntity)entityID;
                Entity entity = rawEntity;
                EditorSystem::SetSelectedEntity(entity);
            }
        }

        // Gizmo stuff
        Entity mainCamera = CameraSystem::GetMainCameraEntity();

        Entity selectedEntity = EditorSystem::GetSelectedEntity();

        if (selectedEntity.IsValid() && mainCamera.IsValid())
        {
            ImGuizmo::SetOrthographic(true);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

            static ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
            static ImGuizmo::MODE mode = ImGuizmo::WORLD;
            static bool bSnapEnabled = false;
            static float snap = 0;

            if (Input::IsKeyPressed(KeyCode::Key_W)) operation = ImGuizmo::TRANSLATE;
            if (Input::IsKeyPressed(KeyCode::Key_E)) operation = ImGuizmo::ROTATE;
            if (Input::IsKeyPressed(KeyCode::Key_R)) operation = ImGuizmo::SCALE;

            auto& cameraComponent = mainCamera.Get<CameraComponent>();
            const float* view = &cameraComponent.ViewMatrix.m[0][0];
            const float* projection = &cameraComponent.ProjectionMatrix.m[0][0];

            TransformComponent& transform = selectedEntity.GetTransform();
            Matrix4 gizmoMatrix = transform.GetMatrix();
            float* gizmoMatrixPtr = &gizmoMatrix.m[0][0];

            ImGuizmo::Manipulate(view, projection, operation, mode, gizmoMatrixPtr, nullptr, bSnapEnabled ? &snap : nullptr);
            
            if (ImGuizmo::IsUsing())
            {
                float matrixTranslation[3], matrixRotation[3], matrixScale[3];
                
                ImGuizmo::DecomposeMatrixToComponents(gizmoMatrixPtr, matrixTranslation, matrixRotation, matrixScale);
                transform.Position = { matrixTranslation[0], matrixTranslation[1], transform.Position.z };
                transform.Rotation = { matrixRotation[0], matrixRotation[1], matrixRotation[2] };
                transform.Scale = { matrixScale[0], matrixScale[1], matrixScale[2] };
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }
}