#pragma once
#include <filesystem>

namespace Nit
{
    using Path = std::filesystem::path;

    TString GetAssetsFolderName();
    Path GetWorkingDirectory();
}