#pragma once
#include "RenderUtils.h"

namespace Nit
{
    struct RenderComponent
    {
        bool    bIsVisible = true;
        Matrix4 Transform;
        int     EntityID = -1;
    };
    
    struct Primitive2DComponent
    {
        Color             TintColor       = Color::White;
        int               SortingLayer    =  0;
        Array<Vector3, 4> VertexPositions = RenderUtils::GetQuadVertexPositions();
        Array<Vector2, 4> VertexUVs       = RenderUtils::GetQuadVertexUVs();
    };

    struct SpriteShapeComponent
    {
        Id      TextureID = 0;
        Vector2 Size      = Vector2::One;
        bool    bFlipX    = false;
        bool    bFlipY    = false;
        Vector2 UVScale   = Vector2::One;    
    };

    struct CircleShapeComponent
    {
        float Radius    = .5f;
        float Thickness = .05f;
        float Fade      = .01f;
    };

    struct LineShapeComponent
    {
        Vector2 Start   = Vector2::Zero;
        Vector2 End     = Vector2::Right;
        float   Thickness = .05f;
        float   Fade      = .01f;
    };
}
