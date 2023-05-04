#include <Nit.h>
#include "TheGameLayer.h"

using namespace Nit;

int main()
{
    Game& game = Game::CreateSingleton();
    game.GetLayerStack()->Push<TheGameLayer>();
    game.Initialize();
}