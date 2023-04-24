#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <string>

namespace Nit
{
    inline std::string CurrentDirectory()
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        const std::string::size_type pos = std::string(buffer).find_last_of("\\/");
        return std::string(buffer).substr(0, pos);
    }
}

