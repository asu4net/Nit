#pragma once
#include "RenderSystem2D.h"

namespace Nit
{
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
    
    class SpriteRenderSystem : public RenderSystem2D<SpriteVertex>
    {
        NIT_RENDER_SYSTEM(SpriteRenderSystem)
    public:
        const ShaderPtr& GetDefaultShader() const override;

    protected:
        void SetBufferElements(VertexBuffer& VBO) const override;
        void SubmitVertices() override;
    };
}