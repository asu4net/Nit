#include "Editor.h"
#include "Game.h"
#include <NitEditor.h>

using namespace Nit;

int main()
{
    Engine& engine = Engine::CreateSingleton();
    engine.GetLayerStack()->Push<Game>();
    engine.GetLayerStack()->Push<Editor>();
    engine.Start();
}