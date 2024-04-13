#pragma once
#include "NitRender/Core/Command.h"
#include "NitRender/Objects/Texture2D.h"
#include "NitRender/Objects/Shader.h"
#include "NitRender/Renderer/Resources.h"
#include "NitRender/Renderer/RenderSystemMap.h"
#include "NitRender/Renderer/SpriteRenderSystem.h"

namespace Nit::Render::Renderer2D
{
    void Init(GraphicsAPI api);
    void Invalidate();
    void StartBatch();
    void NextBatch();
    
    Resources& GetResources();
    RenderSystemMap& GetRenderSystemMap();
    APIPtr GetRenderAPI();
    
    void BindPrimitive(const Primitive2D& primitive);
    int  CalculateTextureSlot(const Texture2DPtr& texture);
    void BindShader(const ShaderPtr& shader);
}