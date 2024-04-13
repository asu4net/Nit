#pragma once
#include "Primitive2D.h"
#include "RenderSystem2D.h"

namespace Nit::Render
{
    class SpriteRenderSystem : public RenderSystem2D<SpriteVertex>
    {
        public: String GetPrimitiveID() const override { return "SpriteRenderSystem"; }; private:
        
    public:
        void SubmitPrimitive(const Primitive2D& primitive2D) override;

    protected:
        void SetBufferElements(VertexBuffer& VBO) const override;
    };
}