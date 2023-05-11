#pragma once
#include "RenderCommandQueue.h"
#include "Data/Texture2D.h"
#include "Camera.h"
#include "Text/Font.h"

namespace Nit
{
    class Camera;
    class Window;

    enum class Flip
    {
        None, X, Y, Both
    };
    
    struct Quad
    {
        mat4 ModelMatrix = MatIdentity;
        vec4 Color = White;
        Shared<Texture2D> Texture = nullptr;
        bool bIsSubTexture = false;
        vec2 SubTextureSize = VecOne;
        vec2 LocationInAtlas = VecZero;
        vec2 UVScale = VecOne;
        vec2 Size = VecOne;
        Flip Flip = Flip::None;
    };

    struct TextQuad
    {
        std::string Text;
        mat4 ModelMatrix = MatIdentity;
        vec4 Color = White;
        Shared<Font> Font = nullptr;
        vec2 Size = VecOne;
        float Spacing = 1;
    };

    struct Renderer2DSettings
    {
        std::string FlatColorShaderLocation = "Content\\Shaders\\FlatColor.glsl";
        std::string TextureShaderLocation = "Content\\Shaders\\Texture.glsl";
    };

    struct RenderData
    {
        Camera RenderCamera;
    };
    
    class Renderer2D : public Singleton<Renderer2D>
    {
    public:
        Shared<Shader> FlatColorShader() const { return m_FlatColorShader; }
        Shared<Shader> TextureShader() const { return m_TextureShader; }
        
        Renderer2D(Renderer2D&&) = delete;
        
        void Start(const Shared<Window>& window, const Renderer2DSettings& rendererSettings = {});
        void Finish();

        void SetRenderData(const RenderData& renderData);
        
        void Begin();
        void End();
        
        const Unique<RenderCommandQueue>& CommandQueue() const { return m_CommandQueue; } 
        
        void ClearScreen(const vec4 clearColor = DarkGrey);
        void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        void SetBlendingMode(const BlendingMode blendingMode);
        void SetDepthTestEnabled(const bool bEnabled);
        void SetBlendingEnabled(const bool bEnabled);
        void SubmitQuad(const Quad& quad);
        void SubmitTextQuad(const TextQuad&  textQuad);

    private:
        Renderer2D() = default;
        
        RenderData m_RenderData;
        Unique<RenderCommandQueue> m_CommandQueue;
        
        Shared<Shader> m_FlatColorShader;
        Shared<Shader> m_TextureShader;

        void StartBatch();
        void Flush();
        void NextBatch();
        void CreateShaders(const Renderer2DSettings& rendererSettings);

        friend class Singleton<Renderer2D>;
    };
}
