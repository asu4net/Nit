#pragma once
#include <filesystem>

namespace Nit
{
    String GetWorkingDirectory();
    String GetAssetsFolderName();
    String GetAssetsDirectory();

    void GetAssetDirectoryLocalPath(const String& filePath, String& localPath);
}