#include "DemoGame.h"

using namespace Nit;

int main()
{
    Game& game = Game::CreateSingleton();
    game.GetLayerStack()->Push<DemoGame>();
    game.Initialize();
}