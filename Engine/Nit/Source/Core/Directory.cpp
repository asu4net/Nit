#include "Directory.h"

namespace Nit
{
    String GetWorkingDirectory()
    {
        static String workingDirectory;
        if (workingDirectory.empty())
        {
            char buffer[MAX_PATH];
            GetModuleFileNameA(nullptr, buffer, MAX_PATH);
            const String::size_type pos = String(buffer).find_last_of("\\/");
            workingDirectory = String(buffer).substr(0, pos);
        }
        return workingDirectory;
    }

    String GetAssetsFolderName()
    {
        static const String s_AssetFolderName = "Assets";
        return s_AssetFolderName;
    }

    String GetAssetsDirectory()
    {
        static const String s_AssetsDirectory = GetWorkingDirectory() + "\\" + GetAssetsFolderName();
        return s_AssetsDirectory;
    }

    void GetAssetDirectoryLocalPath(const String& filePath, String& localPath)
    {
        DynamicArray<String> splitPath;
        String subString;
        std::stringstream ss(filePath);
        bool bProceed = false;
        while (std::getline(ss, subString, '\\'))
        {
            if (subString.find(GetAssetsFolderName()) != String::npos)
                bProceed = true;
            if (bProceed)
                splitPath.push_back("/" + subString);
        }
        for (int i = 0; i < splitPath.size(); i++)
            localPath += splitPath[i];
    }
}