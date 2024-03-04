#pragma once
#include "Nit.h"
#include "GameScripts.h"

using namespace Nit;

int main(int argc, char* argv[])
{
    Engine::Init();
    Engine::Run();
    Engine::Finish();
    return 0;
}