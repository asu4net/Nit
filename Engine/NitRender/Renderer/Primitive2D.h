#pragma once
#include "Objects/Shader.h"
#include "Objects/Texture2D.h"

namespace Nit::Render
{
    struct Primitive2D
    {
        virtual String GetID() const = 0;
        
        bool              bIsVisible      = false;
        Matrix4           Transform;      // Identity
        Matrix4           ProjectionView; // Identity
        float             Time            = 0.f;
        Color             TintColor       = Color::White;
        Array<Vector3, 4> VertexPositions = GetQuadVertexPositions();
        Array<Vector2, 4> VertexUVs       = GetQuadVertexUVs();
        ShaderPtr         ShaderToBind    = nullptr;
        int               EntityID        = -1;
        int               SortingLayer    =  0;
    };
    
    struct SpritePrimitive : Primitive2D
    {
        String GetID() const override { return "SpritePrimitive"; }
        
        Texture2DPtr      TextureToBind = nullptr;
        Vector2           Size          = Vector2::One;
        bool              bFlipX        = false;
        bool              bFlipY        = false;
        Vector2           UVScale       = Vector2::One;
    };
    
    struct SpriteVertex
    {
        Vector3  Position      = Vector3::Zero;
        Vector3  LocalPosition = Vector3::Zero;
        Color    TintColor     = Color::White;
        Vector2  UVCoords      = Vector2::Zero;
        uint32_t TextureSlot   = 0;
        float    Time          = 0.f;
        int      EntityID      = -1;
    };
    
    struct CirclePrimitive : Primitive2D
    {
        String GetID() const override { return "CirclePrimitive"; }
        
        float Radius    = .5f;
        float Thickness = .05f;
        float Fade      = .01f;
    };

    struct CircleVertex
    {
        Vector3  Position      = Vector3::Zero;
        Vector3  LocalPosition = Vector3::Zero;
        Color    TintColor     = Color::White;
        float    Thickness     = .05f;
        float    Fade          = .01f;
        float    Time          =  0.f;
        int      EntityID      = -1;
    };
    
    struct LinePrimitive : Primitive2D
    {
        String GetID() const override { return "LinePrimitive"; }
        
        Vector2 Start   = Vector2::Zero;
        Vector2 End     = Vector2::Right;
        float Thickness = .05f;
        float Fade      = .01f;
    };

    struct LineVertex
    {
        Vector3  Position      = Vector3::Zero;
        Vector3  LocalPosition = Vector3::Zero;
        Color    TintColor     = Color::White;
        Vector2  Size          = Vector2::Zero;
        float    Fade          = .01f;
        float    Time          =  0.f;
        int      EntityID      = -1;
    };
}