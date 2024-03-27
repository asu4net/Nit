#include "Sprite.h"
#include "Core/Engine.h"
#include "Render/Renderer.h"
#include "Render/RendererTexture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <GLAD/glad.h>

// execute cmd command https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po

// Pixel Scanning Algorithm. From https://www.david-colson.com/2020/03/10/exploring-rect-packing.html

/*

struct Rect
{
  int x, y;
  int w, h;
  bool wasPacked = false;
};

void PackRectsBLPixels(std::vector<Rect>& rects)
{
  // Sort by a heuristic
  std::sort(rects.begin(), rects.end(), SortByHeight());

  // Maintain a grid of bools, telling us whether each pixel has got a rect on it
  std::vector<std::vector<bool>> image;
  image.resize(700);
  for (int i=0; i< 700; i++)
  {
    image[i].resize(700, false);
  }

  for (Rect& rect : rects)
  {
    // Loop over X and Y pixels
    bool done = false;
    for( int y = 0; y < 700 && !done; y++)
    {
      for( int x = 0; x < 700 && !done; x++)
      {
        // Make sure this rectangle doesn't go over the edge of the boundary
        if ((y + rect.h) >= 700 || (x + rect.w) >= 700)
          continue;

        // For every coordinate, check top left and bottom right
        if (!image[y][x] && !image[y + rect.h][x + rect.w])
        {
          // Corners of image are free
          // If valid, check all pixels inside that rect
          bool valid = true;
          for (int ix = x; ix < x + rect.w; ix++)
          {
            for (int iy = y; iy < y + rect.h; iy++)
            {
              if (image[iy][ix])
              {
                valid = false;
                break;
              }
            }
          }

          // If all good, we've found a location
          if (valid)
          {
            rect.x = x;
            rect.y = y;
            done = true;

            // Set the used pixels to true so we don't overlap them
            for (int ix = x; ix < x + rect.w; ix++)
            {
              for (int iy = y; iy < y + rect.h; iy++)
              {
                image[iy][ix] = true;
              }
            }

            rect.was_packed = true;
          }
        }
      }
    }
  }
}

*/

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<SubSprite>("SubSprite")
        .constructor<>()
        (
            rttr::policy::ctor::as_object
        )
        .property("Size", &SubSprite::Size)
        .property("VertexPositions", &SubSprite::VertexPositions)
        .property("VertexUVs", &SubSprite::VertexUVs);

    rttr::registration::class_<Sprite>("Sprite")
        .constructor<>()
        .property("MagFilter", &Sprite::m_MagFilter)
        .property("MinFilter", &Sprite::m_MinFilter)
        .property("WrapModeU", &Sprite::m_WrapModeU)
        .property("WrapModeV", &Sprite::m_WrapModeV)
        .property("VertexPositions", &Sprite::m_VertexPositions)
        .property("VertexUVs", &Sprite::m_VertexUVs)
        .property("SubSprites", &Sprite::m_SubSprites);
}

namespace Nit
{
    NIT_FORCE_LINK_IMPL(Sprite)

    String Sprite::DefaultFolder()
    {
        static const String s_SpritesFolder = "Sprites";
        return s_SpritesFolder;
    }

    Sprite::Sprite()
    {
        Init();
    };

    void Sprite::Init(const SpriteInitSettings& initSettings)
    {
        m_MagFilter = initSettings.magFilter;
        m_MinFilter = initSettings.minFilter;
        m_WrapModeU = initSettings.wrapModeU;
        m_WrapModeV = initSettings.wrapModeV;
    }

    bool Sprite::Load()
    {
        const String absolutePath = GetAssetData().AbsolutePath;

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

        RenderUtils::FillQuadVertexPositions(m_Size, m_VertexPositions);
        m_VertexUVs = RenderUtils::GetQuadVertexUVs();

        return true;
    }
    
    void Sprite::Unload()
    {
        stbi_image_free(m_Data);
        m_Data = nullptr;
        Renderer::DestroyTexture2D(m_RendererTextureId);
    }
    
    const SubSprite& Sprite::PushSubSprite(const String& name, const Vector2& uvMin, const Vector2& uvMax, const Vector2& size)
    {
        NIT_CHECK(!m_SubSprites.count(name), "Duplicated name found!");
        SubSprite subSprite;
        subSprite.Size = size;
        RenderUtils::FillQuadVertexPositions(size, subSprite.VertexPositions);
        RenderUtils::FillQuadVertexUVs(uvMin, uvMax, subSprite.VertexUVs);
        m_SubSprites[name] = subSprite;
        return m_SubSprites[name];
    }

    const SubSprite& Sprite::PushSubSprite(const String& name, const Vector2& locationInAtlas, const Vector2& size)
    {
        NIT_CHECK(!m_SubSprites.count(name), "Duplicated name found!");
        
        SubSprite subSprite;
        subSprite.Size = size;

        Vector2 bottomLeft;
        bottomLeft.x = locationInAtlas.x * (1 / m_Size.x);
        bottomLeft.y = 1 - ((locationInAtlas.y + size.y) / m_Size.y);

        Vector2 topRight;
        topRight.x = (locationInAtlas.x + size.x) * (1 / m_Size.x);
        topRight.y = 1 - (locationInAtlas.y / m_Size.y);

        //const Vector2 uvMin((locationInAtlas.x * size.x)       / m_Size.x, (locationInAtlas.y * size.y)       / m_Size.y);
        //const Vector2 uvMax(((locationInAtlas.x + 1) * size.x) / m_Size.x, ((locationInAtlas.y + 1) * size.y) / m_Size.y);
        
        RenderUtils::FillQuadVertexPositions(size, subSprite.VertexPositions);
        RenderUtils::FillQuadVertexUVs(bottomLeft, topRight, subSprite.VertexUVs);
        m_SubSprites[name] = subSprite;
        return m_SubSprites[name];
    }

    void Sprite::PushSpriteSheet(const Vector2& atlasTileSize, uint32_t numOfTiles, bool atlasIsHorizontal)
    {
        for (uint32_t i = 0; i < numOfTiles; ++i)
        {
            Vector2 tileLocation = { (float) i, 0.f };

            if (!atlasIsHorizontal)
            {
                tileLocation = { 0.f, (float) i };
            }

            const String tileName = GetAssetData().Name + " [" + std::to_string(i) + "]";
            PushSubSprite(tileName, tileLocation, atlasTileSize);
        }
    }

    void Sprite::PopSubSprite(const String& name)
    {
        NIT_CHECK(m_SubSprites.count(name), "There is not a SubSprite called %s!", name.c_str());
        m_SubSprites.erase(name);
    }

    const SubSprite& Sprite::GetSubSprite(const String& name)
    {
        NIT_CHECK(m_SubSprites.count(name), "There is not a SubSprite called %s!", name.c_str());
        return m_SubSprites[name];
    }

    bool Sprite::ContainsSubSprite(const String& name)
    {
        return m_SubSprites.count(name);
    }

    void Sprite::ForEachSubSprite(Delegate<void(const String&, const SubSprite&)> eachDelegate)
    {
        for (const auto& [name, subSprite] : m_SubSprites)
        {
            eachDelegate(name, subSprite);
        }
    }
}