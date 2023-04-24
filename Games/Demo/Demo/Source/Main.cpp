#include "DemoGame.h"

int main()
{
    Nit::Game& game = Nit::Game::Create();
    game.PushLayer<DemoGame>();
    game.Start();
}