#include "Directory.h"

namespace Nit
{
    String GetAssetsFolderName()
    {
        return "Assets";
    }
    Path GetWorkingDirectory()
    {
        return std::filesystem::current_path();
    }
}