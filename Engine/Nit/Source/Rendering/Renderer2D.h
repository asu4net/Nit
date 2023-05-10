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
        glm::mat4 ModelMatrix = Math::IdentityMatrix;
        glm::vec4 Color = Math::WhiteColor;
        Shared<Texture2D> Texture = nullptr;
        bool bIsSubTexture = false;
        glm::vec2 SubTextureSize = Math::OneVector;
        glm::vec2 LocationInAtlas = Math::ZeroVector;
        glm::vec2 UVScale = Math::OneVector;
        glm::vec2 Size = Math::OneVector;
        Flip Flip = Flip::None;
    };

    struct TextQuad
    {
        std::string Text;
        glm::mat4 ModelMatrix = Math::IdentityMatrix;
        glm::vec4 Color = Math::WhiteColor;
        Shared<Font> Font = nullptr;
        glm::vec2 Size = Math::OneVector;
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
        
        void ClearScreen(const glm::vec4 clearColor = Math::DarkGreyColor);
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
