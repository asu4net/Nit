#include "EditorLayer.h"
#include "ImGui/ImGuiRenderer.h"

namespace Nit
{
    EditorLayer::EditorLayer()
        : ImGuiRenderer(ImGuiRenderer::CreateSingleton())
    {
    }

    void EditorLayer::OnInitialize()
    {
        ImGuiRenderer.Initialize(Game::GetInstance().GetWindow());
    }

    void EditorLayer::OnUpdate(const TimeStep& timeStep)
    {
        ImGuiRenderer.Update();
    }
}
