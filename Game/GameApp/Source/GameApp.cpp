#pragma once
#include "Nit.h"

using namespace Nit;

int main(int argc, char* argv[])
{
    InitConfig config;
    config.WindowConfiguration.Title = "Test Game :D";
    config.WindowConfiguration.bStartMaximized = true;
    Engine::Init(config);
    Engine::Run();
    Engine::Finish();
    return 0;
}