#include "ContentSystem.h"
#include "Asset/AudioClip.h"
#include "Core\Engine.h"
#include "Asset\Content.h"
#include "Asset\Sprite.h"

namespace Nit::ContentSystem
{
    void OnCreate();

    void Register()
    {
        Engine::CreateSystem("ContentSystem", 5100, ExecutionContext::Editor);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
    }

    String GetRelativeAssetPath(const String& filePath)
    {
        DynamicArray<String> splitPath;
        String subString;
        StringStream ss(filePath);
        bool bProceed{ false };
        while (std::getline(ss, subString, '\\'))
        {
            if (subString.find(Nit::GetAssetsFolderName()) != String::npos)
            {
                bProceed = true;
                continue;
            }
            if (bProceed) splitPath.push_back(subString + "/");
        }
        String res;
        for (int i = 0; i < splitPath.size(); i++)
            res += splitPath[i];
        res = res.substr(0, res.size() - 1);
        return res;
    }

    bool TryImportAsset(const std::filesystem::path& path)
    {
        const String assetName = path.stem().string();

        if (Content::GetAssetByName(assetName).IsValid())
        {
            return false;
        }

        AssetData assetData;
        assetData.Name = assetName;
        assetData.AbsolutePath = path.string();
        assetData.Path = GetRelativeAssetPath(assetData.AbsolutePath);

        AssetRef ref;

        if (path.extension() == ".png" || path.extension() == ".jpg")
        {
            ref = Content::CreateAsset<Sprite>(assetData);
        }
        else if (path.extension() == ".wav")
        {
            ref = Content::CreateAsset<AudioClip>(assetData);
        }
        
        if (!ref.IsValid() || !Content::TryLoadAsset(ref))
        {
            return false;
        }
        else
        {
            SharedPtr<Asset> asset = ref.Get();
            Content::SerializeAsset(asset, "");
            return true;
        }
    }

    void OnCreate()
    {
        std::filesystem::path currentDirectory = Nit::GetWorkingDirectory();

        for (auto& directoryEntry : std::filesystem::recursive_directory_iterator(currentDirectory))
        {
            const auto& path = directoryEntry.path();
            auto relativePath = relative(path, currentDirectory);
            String fileName = relativePath.filename().string();

            if (!directoryEntry.is_directory())
            {
                TryImportAsset(path);
            }
        }
    }
}