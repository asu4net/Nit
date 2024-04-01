#include "Nit.h"
#include "Game.h"

int main(int argc, char* argv[])
{
    using namespace Nit;
    InitConfig config;
    config.WindowConfiguration.Title = "Test Game :D";
    config.WindowConfiguration.bStartMaximized = true;
    Engine::Init(config);
    SpaceInvaders::Register();
    Engine::Run();
    Engine::Finish();
    return 0;
}