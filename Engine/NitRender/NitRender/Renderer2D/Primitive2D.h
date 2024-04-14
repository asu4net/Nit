#pragma once
#include "NitRender/Objects/Shader.h"
#include "NitRender/Objects/Texture2D.h"

namespace Nit
{
    struct CPrimitive2D
    {
        virtual TString GetID() const = 0;
        
        bool                bIsVisible      = false;
        CMatrix4            Transform       = {};      // Identity
        CMatrix4            ProjectionView  = {};      // Identity
        float               Time            = 0.f;
        CColor              TintColor       = CColor::White;
        TArray<CVector3, 4> VertexPositions = GetQuadVertexPositions();
        TArray<CVector2, 4> VertexUVs       = GetQuadVertexUVs();
        TShaderPtr          ShaderToBind    = nullptr;
        int                 EntityID        = -1;
        int                 SortingLayer    =  0;
    };

    struct CSpritePrimitive : CPrimitive2D
    {
        TString GetID() const override { return "SpritePrimitive"; }
        
        TTexture2DPtr      TextureToBind = nullptr;
        CVector2           Size          = CVector2::One;
        bool               bFlipX        = false;
        bool               bFlipY        = false;
        CVector2           UVScale       = CVector2::One;
    };
    
    struct CSpriteVertex
    {
        CVector3  Position      = CVector3::Zero;
        CVector3  LocalPosition = CVector3::Zero;
        CColor    TintColor     = CColor::White;
        CVector2  UVCoords      = CVector2::Zero;
        uint32_t  TextureSlot   = 0;
        float     Time          = 0.f;
        int       EntityID      = -1;
    };
    
    struct CCirclePrimitive : CPrimitive2D
    {
        TString GetID() const override { return "CirclePrimitive"; }
        
        float Radius    = .5f;
        float Thickness = .05f;
        float Fade      = .01f;
    };

    struct CCircleVertex
    {
        CVector3  Position      = CVector3::Zero;
        CVector3  LocalPosition = CVector3::Zero;
        CColor    TintColor     = CColor::White;
        float     Thickness     = .05f;
        float     Fade          = .01f;
        float     Time          =  0.f;
        int       EntityID      = -1;
    };
    
    struct CLinePrimitive : CPrimitive2D
    {
        TString GetID() const override { return "LinePrimitive"; }
        
        CVector2 Start      = CVector2::Zero;
        CVector2 End        = CVector2::Right;
        float    Thickness  = .05f;
        float    Fade       = .01f;
    };

    struct CLineVertex
    {
        CVector3  Position      = CVector3::Zero;
        CVector3  LocalPosition = CVector3::Zero;
        CColor    TintColor     = CColor::White;
        CVector2  Size          = CVector2::Zero;
        float     Fade          = .01f;
        float     Time          =  0.f;
        int       EntityID      = -1;
    };
}
