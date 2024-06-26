#include "Nit.h"
#include "NitEditor.h"
#include "Game.h"

using namespace Nit;

int main(int argc, char* argv[])
{
    InitConfig config;

    //config.WindowConfiguration.VSync = false;
    config.WindowConfiguration.Title = "Nit Editor";
    config.WindowConfiguration.bStartMaximized = true;
    
    Engine::Init(config);
    
    EditorSystem::Register();
    SpaceInvaders::Register();

    RunConfig runConfig;
    runConfig.bStartStopped = true;
    Engine::Run(runConfig);
    
    Engine::Finish();
    return 0;
}