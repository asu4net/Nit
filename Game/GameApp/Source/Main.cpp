#include <Nit.h>
#include "Game.h"

using namespace Nit;

int main()
{
    Engine& engine = Engine::CreateSingleton();
    engine.GetLayerStack()->Push<Game>();
    engine.Start();
}