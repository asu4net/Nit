#include "NitPCH.h"
#include "AssetManager.h"

namespace Nit
{
    static std::string GetAssetFolder(const std::string& path)
    {
        std::vector<std::string> splitPath;
        std::string subString;
        std::stringstream ss(path);
        while(std::getline(ss, subString, '/'))
            splitPath.push_back(subString);
        splitPath.erase(splitPath.begin() + (splitPath.size() - 1));
        std::string res;
        for (int i = 0; i < splitPath.size(); i++)
            res += splitPath[i];
        return res;
    }
    
    void AssetManager::Initialize()
    {
    }

    void AssetManager::Finalize()
    {
    }

    Id AssetManager::SaveAsset(const std::string& path, const std::string& type, const std::string& name)
    {
        return 0;
    }
}
