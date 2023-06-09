#include "NitPCH.h"
#include "AssetManager.h"
#include "Audio/AudioClip.h"
#include "Core/Serialization.h"
#include "Rendering/Data/Shader.h"
#include "Rendering/Data/Texture2D.h"
#include "Rendering/Text/Font.h"

namespace Nit
{
    bool AssetManager::SerializeAsset(const Shared<Asset>& asset)
    {
        const std::string jsonAssetInfo = Serialization::ToJson(asset);

        std::ofstream fileAssetInfo("../" + asset->GetPath() + ".assetInfo"); //TODO: Use display name
        std::ofstream fileAssetInfoExe(asset->GetPath() + ".assetInfo"); //TODO: Use display name
        std::ofstream fileAssetAbs(asset->GetAbsolutePath() + ".assetInfo");
        
        fileAssetInfo << jsonAssetInfo;
        fileAssetInfoExe << jsonAssetInfo;
        fileAssetAbs << jsonAssetInfo;
        
        return true;
    }

    bool AssetManager::DeserializeAsset(const std::filesystem::path& assetInfoPath)
    {
        if (assetInfoPath.extension() != ".assetInfo") return false;
        
        const std::ifstream stream(assetInfoPath);

        if (stream.fail()) return false;
        
        std::stringstream ss;
        ss << stream.rdbuf();

        // AssetInfo assetInfo;
        Asset assetData;
        Serialization::FromJson(ss.str(), assetData);
                
        rttr::type assetType = rttr::type::get_by_name(assetData.GetTypeName());
                
        if (!assetType.is_valid())
            return false; ////TODO: destroy asset info file
                
        rttr::variant variant = assetType.create({assetData.GetName(), assetData.GetPath(), assetData.GetId()});

        if (!variant.is_valid())
            return false; ////TODO: destroy asset info file
                
        rttr::instance assetInstance = variant;
        Serialization::FromJson(ss.str(), assetInstance);
        Shared<Asset> asset = variant.get_value<Shared<Asset>>();

        if (!asset->Load()) //TODO: destroy asset info file
            return false;
        
        asset->Initialize();

        m_IdAssetMap[asset->GetId()] = asset;
        m_NameIdMap[asset->GetName()] = asset->GetId();
        return true;
    }

    static std::string GetRelativeAssetPath(const std::string& filePath)
    {
        std::vector<std::string> splitPath;
        std::string subString;
        std::stringstream ss(filePath);
        bool bProceed{false};
        while(std::getline(ss, subString, '\\'))
        {
            if (subString.contains("Content")) bProceed = true;
            if (bProceed) splitPath.push_back("/" + subString);
        }
        std::string res;
        for (int i = 0; i < splitPath.size(); i++)
            res += splitPath[i];
        return res;
    }

    Weak<Asset> AssetManager::GetAssetById(const Id& id)
    {
        if (!m_IdAssetMap.contains(id))
            return {};
        return m_IdAssetMap[id];
    }

    bool AssetManager::ImportAsset(const std::filesystem::path& path)
    {
        if (path.extension() == ".png" || path.extension() == ".jpg")
        {
            const std::string folder = GetRelativeAssetPath(path.string());
            auto textureLink = CreateAsset<Texture2D>(path.stem().string(), folder);
            if (!textureLink.IsValid()) return false;
            auto texture = textureLink.GetAs<Texture2D>();
            SerializeAsset(texture);
            texture->UploadToGPU();
            return true;
        }
        if (path.extension() == ".glsl")
        {
            const std::string folder = GetRelativeAssetPath(path.string());
            auto shaderLink = CreateAsset<Shader>(path.stem().string(), folder);
            if (!shaderLink.IsValid()) return false;
            auto shader = shaderLink.GetAs<Shader>();
            SerializeAsset(shader);
            shader->Compile();
        }
        if (path.extension() == ".wav")
        {
            const std::string folder = GetRelativeAssetPath(path.string());
            auto audioLink = CreateAsset<AudioClip>(path.stem().string(), folder);
            if (!audioLink.IsValid()) return false;
            auto audio = audioLink.GetAs<AudioClip>();
            SerializeAsset(audio);
            audio->Initialize();
        }
        if (path.extension() == ".ttf")
        {
            const std::string folder = GetRelativeAssetPath(path.string());
            auto fontLink = CreateAsset<Font>(path.stem().string(), folder);
            if (!fontLink.IsValid()) return false;
            auto font = fontLink.GetAs<Font>();
            SerializeAsset(font);
            font->Initialize();
        }
        return false;
    }

    AssetLink AssetManager::GetAssetByName(const std::string& assetName)
    {
        if (!m_NameIdMap.contains(assetName))
            return {};

        const Shared<Asset> asset = m_IdAssetMap[m_NameIdMap[assetName]];
        AssetLink link = {asset->GetName(), asset->GetId(),
            asset->get_type().get_name().to_string()};
        return link;
    }

    void AssetManager::Start()
    {
        static const std::string AssetDirectory = CurrentDirectory() + "\\Content";
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(AssetDirectory))
        {
            const std::filesystem::path& dirPath = dirEntry.path();
            if (dirEntry.is_directory()) continue;
            DeserializeAsset(dirPath);
        }
    }

    void AssetManager::Finish()
    {
        for (auto[id, asset] : m_IdAssetMap)
        {
            if (asset->GetPath() != "None")
            {
                SerializeAsset(asset);
            }
            assert(asset->Unload());
        }
    }
}
