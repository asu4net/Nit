#include "SpritePrimitive.h"
#include "Asset/Sprite.h"

namespace Nit
{
    SpritePrimitive::SpritePrimitive()  = default;
    SpritePrimitive::~SpritePrimitive() = default;

    void SpritePrimitive::GenerateVertexData(SpriteShape shape)
    {
        switch (shape)
        {
        case Nit::SpriteShape::Defaut:
            GenerateSpriteVertexData();
            break;
        case Nit::SpriteShape::Circle:
            GenerateCircleVertexData();
            break;
        case Nit::SpriteShape::Rect:
            GenerateRectVertexData();
            break;
        case Nit::SpriteShape::Line:
            GenerateLineVertexData();
            break;
        default:
            NIT_CHECK(false, "Unhandled Fragment Shape");
            break;
        }
    }

    void SpritePrimitive::GenerateSpriteVertexData()
    {
        m_FragmentShape = FragmentShape::Defaut;
        UpdateLocalVertexPositions();
        UpdateVertexPositions();
        UpdateVertexUVs();
        UpdateVertexColors();
    }

    void SpritePrimitive::GenerateCircleVertexData()
    {
        m_FragmentShape = FragmentShape::Circle;
        Size = Vector2::One * Radius * 2.f;
        UpdateLocalVertexPositions();
        UpdateVertexPositions();
        UpdateVertexUVs();
        UpdateVertexColors();
    }

    void SpritePrimitive::GenerateLineVertexData()
    {
        m_FragmentShape = FragmentShape::Defaut;
        UpdateVertexUVs();
        m_VertexPositions[0] = StartPosition + NormalVector *  Thickness / 2;
        m_VertexPositions[1] = EndPosition   + NormalVector *  Thickness / 2;
        m_VertexPositions[2] = EndPosition   + NormalVector * -Thickness / 2;
        m_VertexPositions[3] = StartPosition + NormalVector * -Thickness / 2;
        UpdateVertexColors();
    }

    void SpritePrimitive::GenerateRectVertexData()
    {
        m_FragmentShape = FragmentShape::Rect;
        UpdateVertexUVs();
        UpdateLocalVertexPositions();
        UpdateVertexPositions();
        UpdateVertexColors();

        for (const Vector3& vertexPosition : m_LocalVertexPositions)
        {
            const float x = std::abs(vertexPosition.x);
            const float y = std::abs(vertexPosition.y);
            if (x > Bounds.x) Bounds.x = x;
            if (y > Bounds.y) Bounds.y = y;
        }
    }
    
    void SpritePrimitive::UpdateLocalVertexPositions()
    {
        if (SpriteRef)
        {
            switch (Source)
            {
            case SpriteSource::Default:
                m_LocalVertexPositions = SpriteRef->GetVertexPositions();
                return;
            case SpriteSource::SubSprite:
            {
                if (!SpriteRef->ContainsSubSprite(SubSpriteName))
                {
                    m_LocalVertexPositions = SpriteRef->GetVertexPositions();
                    return;
                }
                m_LocalVertexPositions = SpriteRef->GetSubSprite(SubSpriteName).VertexPositions;
            }
                return;
            default:
                NIT_CHECK(false, "Invalid source!");
                return;
            }
        }

        m_LocalVertexPositions = Math::GetDefaultQuadVertexPositions();
    }
    
    void SpritePrimitive::UpdateVertexPositions()
    {
        for (uint32_t i = 0; i < 4; i++)
        {
            m_VertexPositions[i] = Vector4(m_LocalVertexPositions[i], 1.f) * TransformMatrix;
            m_VertexPositions[i].x *= Size.x;
            m_VertexPositions[i].y *= Size.y;
        }
    }

    void SpritePrimitive::UpdateVertexUVs()
    {
        if (SpriteRef)
        {
            switch (Source)
            {
            case SpriteSource::Default:
                m_VertexUVs = SpriteRef->GetVertexUVs();
                break;
            case SpriteSource::SubSprite:
            {
                if (!SpriteRef->ContainsSubSprite(SubSpriteName))
                {
                    m_VertexUVs = SpriteRef->GetVertexUVs();
                    break;
                }
                m_VertexUVs = SpriteRef->GetSubSprite(SubSpriteName).VertexUVs;
            }
                break;
            default:
                NIT_CHECK(false, "Invalid source!");
                break;
            }
        }
        else
        {
            m_VertexUVs = Math::GetDefaultQuadUVs();
        }

        for (Vector2& vertexUV : m_VertexUVs)
        {
            vertexUV.x *= UVScale.x;
            vertexUV.y *= UVScale.y;
        }

        Math::FlipQuadVertexUVs(FlipMode, m_VertexUVs);
    }

    void SpritePrimitive::UpdateVertexColors()
    {
        for (Color& vertexColor : m_VertexColors)
            vertexColor = TintColor;
    }
}