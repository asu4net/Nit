#include "Sprite.h"
#include "Core/Engine.h"
#include "Render/Renderer.h"
#include "Render/RendererTexture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <GLAD/glad.h>

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<CSubSprite>("SubSprite")
        .constructor<>()
        (
            rttr::policy::ctor::as_object
        )
        .property("Size", &CSubSprite::Size)
        .property("VertexPositions", &CSubSprite::VertexPositions)
        .property("VertexUVs", &CSubSprite::VertexUVs);

    rttr::registration::class_<CSprite>("Sprite")
        .constructor<>()
        .property("MagFilter", &CSprite::m_MagFilter)
        .property("MinFilter", &CSprite::m_MinFilter)
        .property("WrapModeU", &CSprite::m_WrapModeU)
        .property("WrapModeV", &CSprite::m_WrapModeV)
        .property("VertexPositions", &CSprite::m_VertexPositions)
        .property("VertexUVs", &CSprite::m_VertexUVs)
        .property("SubSprites", &CSprite::m_SubSprites);
}

namespace Nit
{
    NIT_FORCE_LINK_IMPL(CSprite)

    TString CSprite::DefaultFolder()
    {
        static const TString s_SpritesFolder = "Sprites";
        return s_SpritesFolder;
    }

    CSprite::CSprite()
    {
        Init();
    };

    void CSprite::Init(const CSpriteInitSettings& initSettings)
    {
        m_MagFilter = initSettings.MagFilter;
        m_MinFilter = initSettings.MinFilter;
        m_WrapModeU = initSettings.WrapModeU;
        m_WrapModeV = initSettings.WrapModeV;
    }

    bool CSprite::Load()
    {
        if (m_Data)
        {
            Unload();
        }
        
        const TString absolutePath = GetAssetData().AbsolutePath;

        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        m_Data = stbi_load(absolutePath.c_str(), &width, &height, &channels, 0);

        if (!m_Data)
            return false;

        m_Size = { (float) width, (float) height };
        m_Channels = channels;

        Texture2DSettings settings;
        settings.Width = width;
        settings.Height = height;
        settings.Channels = m_Channels;
        settings.MagFilter = m_MagFilter;
        settings.MinFilter = m_MinFilter;
        settings.WrapModeU = m_WrapModeU;
        settings.WrapModeV = m_WrapModeV;

        m_RendererTextureId = Renderer::CreateTexture2D(settings, m_Data);

        Render::FillQuadVertexPositions(m_Size, m_VertexPositions);
        m_VertexUVs = Render::GetQuadVertexUVs();

        return true;
    }
    
    void CSprite::Unload()
    {
        stbi_image_free(m_Data);
        m_Data = nullptr;
        Renderer::DestroyTexture2D(m_RendererTextureId);
    }
    
    const CSubSprite& CSprite::PushSubSprite(const TString& name, const CVector2& uvMin, const CVector2& uvMax, const CVector2& size)
    {
        NIT_CHECK(!m_SubSprites.count(name), "Duplicated name found!");
        CSubSprite subSprite;
        subSprite.Size = size;
        Render::FillQuadVertexPositions(size, subSprite.VertexPositions);
        Render::FillQuadVertexUVs(uvMin, uvMax, subSprite.VertexUVs);
        m_SubSprites[name] = subSprite;
        return m_SubSprites[name];
    }

    const CSubSprite& CSprite::PushSubSprite(const TString& name, const CVector2& locationInAtlas, const CVector2& size)
    {
        NIT_CHECK(!m_SubSprites.count(name), "Duplicated name found!");
        
        CSubSprite subSprite;
        subSprite.Size = size;

        CVector2 bottomLeft;
        bottomLeft.x = locationInAtlas.x * (1 / m_Size.x);
        bottomLeft.y = 1 - ((locationInAtlas.y + size.y) / m_Size.y);

        CVector2 topRight;
        topRight.x = (locationInAtlas.x + size.x) * (1 / m_Size.x);
        topRight.y = 1 - (locationInAtlas.y / m_Size.y);
        
        Render::FillQuadVertexPositions(size, subSprite.VertexPositions);
        Render::FillQuadVertexUVs(bottomLeft, topRight, subSprite.VertexUVs);
        m_SubSprites[name] = subSprite;
        
        return m_SubSprites[name];
    }

    void CSprite::PushSpriteSheet(const CVector2& atlasTileSize, uint32_t numOfTiles, bool atlasIsHorizontal)
    {
        for (uint32_t i = 0; i < numOfTiles; ++i)
        {
            CVector2 tileLocation = { (float) i, 0.f };

            if (!atlasIsHorizontal)
            {
                tileLocation = { 0.f, (float) i };
            }

            const TString tileName = GetAssetData().Name + " [" + std::to_string(i) + "]";
            PushSubSprite(tileName, tileLocation, atlasTileSize);
        }
    }

    void CSprite::PopSubSprite(const TString& name)
    {
        NIT_CHECK(m_SubSprites.count(name), "There is not a SubSprite called %s!", name.c_str());
        m_SubSprites.erase(name);
    }

    const CSubSprite& CSprite::GetSubSprite(const TString& name)
    {
        NIT_CHECK(m_SubSprites.count(name), "There is not a SubSprite called %s!", name.c_str());
        return m_SubSprites[name];
    }

    bool CSprite::ContainsSubSprite(const TString& name)
    {
        return m_SubSprites.count(name);
    }

    void CSprite::ForEachSubSprite(Delegate<void(const TString&, const CSubSprite&)> eachDelegate)
    {
        for (const auto& [name, subSprite] : m_SubSprites)
        {
            eachDelegate(name, subSprite);
        }
    }

    void CSprite::ClearSubSprites()
    {
        m_SubSprites.clear();
    }
}
