#pragma once
#include "Utility/Singleton.h"
#include "RenderCommandQueue.h"
#include "Math/Math.h"
#include <memory>

#include "Camera.h"

namespace Nit
{
    class Camera;
    class Window;

    struct Quad
    {
        glm::mat4 ModelMatrix = Global::IdentityMatrix;
        glm::vec4 Color = Global::WhiteColor;
        std::shared_ptr<Texture> Texture = nullptr;
        bool bIsSubTexture = false;
        glm::vec2 SubTextureSize = Global::OneVector;
        glm::vec2 LocationInAtlas = Global::ZeroVector;
        glm::vec2 UVScale = Global::OneVector;
        glm::vec2 Size = Global::OneVector;
    };

    struct Renderer2DSettings
    {
        bool bReadShadersFromFiles = true;
        std::string FlatColorShaderLocation = "Content/Shaders/FlatColor.glsl";
        std::string TextureShaderLocation = "Content/Shaders/Texture.glsl";
    };

    struct RenderData
    {
        Camera RenderCamera;
    };
    
    class Renderer2D : public Singleton<Renderer2D>
    {
    public:
        std::shared_ptr<Shader> FlatColorShader() const { return m_FlatColorShader; }
        std::shared_ptr<Shader> TextureShader() const { return m_TextureShader; }

        Renderer2D() = default;
        Renderer2D(Renderer2D&&) = delete;

        static Renderer2D& CreateAndInitialize(const std::shared_ptr<Window>& window);
        static void FinalizeAndDestroy();
        
        void Initialize(const Renderer2DSettings& rendererSettings = {});
        void Finalize();

        void SetRenderData(const RenderData& renderData);
        
        void Begin();
        void End();
        
        const std::unique_ptr<RenderCommandQueue>& CommandQueue() const { return m_CommandQueue; } 
        
        void ClearScreen(const glm::vec4 clearColor = Global::DarkGreyColor);
        void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        void SetBlendingMode(const BlendingMode blendingMode);
        void SetDepthTestEnabled(const bool bEnabled);
        void SetBlendingEnabled(const bool bEnabled);
        void SubmitQuad(const Quad& quadProperties);

    private:
        RenderData m_RenderData;
        std::unique_ptr<RenderCommandQueue> m_CommandQueue;
        
        std::shared_ptr<Shader> m_FlatColorShader;
        std::shared_ptr<Shader> m_TextureShader;

        void StartBatch();
        void Flush();
        void NextBatch();
        void CreateShaders(const Renderer2DSettings& rendererSettings);
    };
}
