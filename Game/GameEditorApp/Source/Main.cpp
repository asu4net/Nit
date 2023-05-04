#include "EditorLayer.h"
#include "TheGameLayer.h"
#include <NitEditor.h>

using namespace Nit;

int main()
{
    Game& game = Game::CreateSingleton();
    game.GetLayerStack()->Push<TheGameLayer>();
    game.GetLayerStack()->Push<EditorLayer>();
    game.Initialize();
}