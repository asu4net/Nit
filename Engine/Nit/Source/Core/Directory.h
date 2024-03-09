#pragma once
#include <filesystem>

namespace Nit
{
    using Path = std::filesystem::path;

    String GetAssetsFolderName();
    Path GetWorkingDirectory();
}