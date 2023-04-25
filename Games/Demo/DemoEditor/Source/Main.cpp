#include "EditorLayer.h"
#include <DemoGame.h>
#include <NitEditor.h>

using namespace Nit;

class DemoEditorLayer : public EditorLayer
{
    void OnInitialize() override
    {
        EditorLayer::OnInitialize();
        const auto gameLayer = Game::GetInstance().GetLayerStack()->Get<DemoGame>();
        if (!gameLayer) return;
        ImGuiRenderer.PushWidget<Vector3Widget>(gameLayer->BallPosition, "Ball Pos");
    }
};

int main()
{
    Game& game = Game::CreateSingleton();
    game.GetLayerStack()->Push<DemoGame>();
    game.GetLayerStack()->Push<DemoEditorLayer>();
    game.Initialize();
}