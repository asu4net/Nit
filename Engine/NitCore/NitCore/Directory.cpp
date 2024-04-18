#include "Directory.h"

namespace Nit
{
    TString GetAssetsFolderName()
    {
        return "Assets";
    }
    Path GetWorkingDirectory()
    {
        return std::filesystem::current_path();
    }
}