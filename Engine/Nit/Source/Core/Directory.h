#pragma once
#include <filesystem>

namespace Nit
{
    String GetWorkingDirectory();
    String GetAssetsFolderName();
    void GetAssetDirectoryLocalPath(const String& filePath, String& localPath);
}