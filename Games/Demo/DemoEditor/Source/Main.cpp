#include "EditorLayer.h"
#include <DemoGame.h>
#include <NitEditor.h>

using namespace Nit;

int main()
{
    Game& game = Game::CreateSingleton();
    game.GetLayerStack()->Push<DemoGame>();
    game.GetLayerStack()->Push<EditorLayer>();
    game.Initialize();
}