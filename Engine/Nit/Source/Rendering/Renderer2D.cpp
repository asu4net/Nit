#include "Renderer2D.h"
#include "Window/Window.h"
#include "RenderCommand.h"
#include "RenderCommandQueue.h"
#include "Data/IndexBuffer.h"
#include "Data/VertexArray.h"
#include "Data/Shader.h"
#include "Data/Texture2D.h"
#include "Data/VertexBuffer.h"
#include "RawShaderStrings.h"
#include "Core/Asset/AssetManager.h"

namespace Nit
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 UV;
        glm::vec2 UVScale;
        uint32_t TextureSlot;
    };

    struct QuadRenderData
    {
        static constexpr uint32_t MaxQuads = 10000;
        static constexpr uint32_t MaxTextureSlots = 32;

        static constexpr uint32_t MaxVertices = MaxQuads * 4;

        Shared<VertexArray> VertexArray;
        Shared<VertexBuffer> VertexBuffer;
        Shared<IndexBuffer> IndexBuffer;

        QuadVertex* VertexData = nullptr;
        QuadVertex* LastVertex = nullptr;
        uint32_t QuadCount = 0;

        std::array<Shared<Texture2D>, MaxTextureSlots> Textures;
        Shared<Texture2D> WhiteTexture;
        uint32_t LastTextureSlot = 1;
        uint32_t IndexCount = 0;
    };

    static QuadRenderData g_QuadRenderData;
    static int32_t g_TextureSlots[QuadRenderData::MaxTextureSlots];

    static void GetQuadVertexPositions(const glm::vec2& textureSize, const glm::vec2& quadSize, std::array<glm::vec3, 4>& vertexPositions)
    {
        glm::vec2 vertexPosition = Math::OneVector;

        if (glm::abs(textureSize.x - textureSize.y) > 0.0001f)
            vertexPosition = glm::normalize(glm::vec2(textureSize));
        
        vertexPosition.x /= 2;
        vertexPosition.y /= 2;
        vertexPosition.x *= quadSize.y;
        vertexPosition.y *= quadSize.y;
        
        vertexPositions =
        {
            glm::vec3{-vertexPosition.x, -vertexPosition.y, 0.f},
            glm::vec3{ vertexPosition.x, -vertexPosition.y, 0.f},
            glm::vec3{vertexPosition.x, vertexPosition.y, 0.f},
            glm::vec3{-vertexPosition.x, vertexPosition.y, 0.f},
        };
    }

    static void GetQuadVertexUV(const glm::vec2& subTexSize, const glm::vec2& atlasSize, const glm::vec2& location,
        std::array<glm::vec2, 4>& vertexUV, Flip flip)
    {
        const float x{location.x}, y{location.y};
        const float atlasWidth = atlasSize.x;
        const float atlasHeight = atlasSize.y;
        const float subTexWidth = subTexSize.x;
        const float subTexHeight = subTexSize.y;

        const std::array uv = {
            glm::vec2{(x * subTexWidth) / atlasWidth, (y * subTexHeight) / atlasHeight },
            glm::vec2{((x + 1) * subTexWidth) / atlasWidth, (y * subTexHeight) / atlasHeight },
            glm::vec2{((x + 1) * subTexWidth) / atlasWidth, ((y + 1) * subTexHeight) / atlasHeight },
            glm::vec2{(x * subTexWidth) / atlasWidth, ((y + 1) * subTexHeight) / atlasHeight },
        };

        switch (flip)
        {
        case Flip::None:
            vertexUV = uv;
            return;
        case Flip::X:
            vertexUV[0] = uv[1];
            vertexUV[1] = uv[0];
            vertexUV[2] = uv[3];
            vertexUV[3] = uv[2];
            return;
        case Flip::Y:
            vertexUV[0] = uv[2];
            vertexUV[1] = uv[3];
            vertexUV[2] = uv[0];
            vertexUV[3] = uv[1];
            return;
        case Flip::Both:
            vertexUV[0] = uv[3];
            vertexUV[1] = uv[2];
            vertexUV[2] = uv[1];
            vertexUV[3] = uv[0];
        }
    }
    
    void Renderer2D::CreateShaders(const Renderer2DSettings& rendererSettings)
    {
        AssetManager& assetManager = AssetManager::GetInstance();

        AssetLink<Shader> flatColorShaderLink = assetManager.GetAssetByName<Shader>("FlatColorShader");
        
        if (flatColorShaderLink.IsValid())
            m_FlatColorShader = flatColorShaderLink.Lock();
        
        else
        {
            m_FlatColorShader = CreateShared<Shader>();
            m_FlatColorShader->Compile(g_FlatColorVertexShaderSource, g_FlatColorFragmentShaderSource);
            printf("FlatColorShader: Using raw string shader source.\n");
        }

        AssetLink<Shader> textureShaderLink = assetManager.GetAssetByName<Shader>("TextureShader");
        
        if (textureShaderLink.IsValid())
            m_TextureShader = textureShaderLink.Lock();
        
        else
        {
            m_TextureShader = CreateShared<Shader>();
            m_TextureShader->Compile(g_TextureVertexShaderSource, g_TextureFragmentShaderSource);
            printf("TextureShader: Using raw string shader source.\n");
        }
    }
    
    void Renderer2D::Initialize(const Shared<Window>& window, const Renderer2DSettings& rendererSettings)
    {
        AssetManager& assetManager = AssetManager::GetInstance();
        m_CommandQueue = std::make_unique<RenderCommandQueue>();
        SetBlendingEnabled(true);
        SetBlendingMode(BlendingMode::Alpha);
        CreateShaders(rendererSettings);
        
        g_QuadRenderData.VertexData = new QuadVertex[QuadRenderData::MaxVertices];
        g_QuadRenderData.LastVertex = g_QuadRenderData.VertexData;
        auto whiteTexture = assetManager.CreateAsset<Texture2D>("WhiteTexture", "");
        g_QuadRenderData.WhiteTexture = whiteTexture.Lock();
        
        Texture2DSettings settings;
        settings.CreateFromFile = false;
        settings.Width = 1;
        settings.Height = 1;

        g_QuadRenderData.WhiteTexture->Configure(settings);
        g_QuadRenderData.WhiteTexture->UploadToGPU();
        constexpr uint32_t whiteTextureData = 0xffffffff;
        g_QuadRenderData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
        g_QuadRenderData.Textures[0] = g_QuadRenderData.WhiteTexture;

        g_QuadRenderData.VertexArray = VertexArray::Create();
        g_QuadRenderData.VertexBuffer = VertexBuffer::Create(QuadRenderData::MaxVertices * sizeof(QuadVertex));
        g_QuadRenderData.VertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float2, "a_UV"},
            {ShaderDataType::Float2, "a_UVScale"},
            {ShaderDataType::Float, "a_TextureSlot"}
        });
        g_QuadRenderData.VertexArray->AddVertexBuffer(g_QuadRenderData.VertexBuffer);

        constexpr uint32_t maxIndices = QuadRenderData::MaxQuads * 6;

        uint32_t* indices = new uint32_t[maxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < maxIndices; i += 6)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;

            offset += 4;
        }

        g_QuadRenderData.IndexBuffer = IndexBuffer::Create(indices, maxIndices);
        g_QuadRenderData.VertexArray->SetIndexBuffer(g_QuadRenderData.IndexBuffer);

        delete[] indices;

        for (uint32_t i = 0; i < QuadRenderData::MaxTextureSlots; i++)
            g_TextureSlots[i] = i;

        window->Events().ResizeEvent.Add([](const int width, const int height)
        {
            GetInstance().SetViewPort(0, 0, width, height);
        });
    }

    void Renderer2D::Finalize()
    {
    }

    void Renderer2D::SetRenderData(const RenderData& renderData)
    {
        m_RenderData = renderData;
    }

    int GetTextureSlot(const Shared<Texture2D>& texture)
    {
        int textureSlot = 0;

        if (texture)
        {
            // Search existing texture
            for (uint32_t i = 1; i < g_QuadRenderData.LastTextureSlot; i++)
            {
                if (g_QuadRenderData.Textures[i]->GetTextureID() == texture->GetTextureID())
                {
                    textureSlot = i;
                    break;
                }
            }

            // If not exists save the new texture
            if (textureSlot == 0)
            {
                g_QuadRenderData.Textures[g_QuadRenderData.LastTextureSlot] = texture;
                textureSlot = g_QuadRenderData.LastTextureSlot;
                g_QuadRenderData.LastTextureSlot++;
            }
        }

        return textureSlot;
    }

    void Renderer2D::SubmitQuad(const Quad& quad)
    {
        if (g_QuadRenderData.QuadCount > QuadRenderData::MaxQuads || g_QuadRenderData.LastTextureSlot > QuadRenderData::MaxTextureSlots)
            NextBatch();
        
        std::array<glm::vec3, 4> vertexPositions{};

        const float textureWidth = quad.Texture ? static_cast<float>(quad.Texture->GetWidth()) : 1;
        const float textureHeight = quad.Texture ? static_cast<float>(quad.Texture->GetHeight()) : 1;
        const glm::vec2 textureSize = {textureWidth, textureHeight};
        
        GetQuadVertexPositions(textureSize, quad.Size, vertexPositions);
        
        std::array<glm::vec2, 4> vertexUV{};

        const glm::vec2 subTexSize = quad.bIsSubTexture ? quad.SubTextureSize : textureSize;
        GetQuadVertexUV(subTexSize, textureSize, quad.LocationInAtlas, vertexUV, quad.Flip);
        
        for (int i = 0; i < 4; i++)
        {
            g_QuadRenderData.LastVertex->Position = quad.ModelMatrix * glm::vec4(vertexPositions[i], 1.0f);
            g_QuadRenderData.LastVertex->Color = quad.Color;
            g_QuadRenderData.LastVertex->UV = vertexUV[i];
            g_QuadRenderData.LastVertex->UVScale = quad.UVScale;
            g_QuadRenderData.LastVertex->TextureSlot = GetTextureSlot(quad.Texture);
            g_QuadRenderData.LastVertex++;
        }

        g_QuadRenderData.IndexCount += 6;
        g_QuadRenderData.QuadCount++;
    }

    void Renderer2D::SubmitTextQuad(const TextQuad& textQuad)
    {
        const Shared<Texture2D> atlas = textQuad.Font->GetFontAtlas();

        glm::vec3 offset = Math::ZeroVector;
        
        for (const char c : textQuad.Text)
        {
            AlignedQuad q{};
            textQuad.Font->GetBakedChar(c, q);
            std::array<glm::vec3, 4> vertexPositions{};
            std::array<glm::vec2, 4> vertexUV{};

            vertexPositions[0] = {q.X0, q.Y0, 0}; vertexUV[3] = {q.S0, q.T0};
            vertexPositions[1] = {q.X1, q.Y0, 0}; vertexUV[2] = {q.S1, q.T0};
            vertexPositions[2] = {q.X1, q.Y1, 0}; vertexUV[1] = {q.S1, q.T1};
            vertexPositions[3] = {q.X0, q.Y1, 0}; vertexUV[0] = {q.S0, q.T1};

            static constexpr float scale = 0.005f;
            
            glm::mat4 transform = glm::translate(textQuad.ModelMatrix, offset);
            transform *= glm::scale(Math::IdentityMatrix, {scale, scale, scale});
            
            for (int i = 0; i < 4; i++)
            {
                g_QuadRenderData.LastVertex->Position = transform * glm::vec4(vertexPositions[i], 1.0f);
                g_QuadRenderData.LastVertex->Color = textQuad.Color;
                g_QuadRenderData.LastVertex->UV = vertexUV[i];
                g_QuadRenderData.LastVertex->UVScale = Math::WhiteColor;
                g_QuadRenderData.LastVertex->TextureSlot = GetTextureSlot(atlas);
                g_QuadRenderData.LastVertex++;
            }

            g_QuadRenderData.IndexCount += 6;
            g_QuadRenderData.QuadCount++;

            offset.x += q.XPos * scale;
            offset.y += q.YPos * scale;
        }
    }

    void Renderer2D::Flush()
    {
        g_QuadRenderData.VertexBuffer->SetData(g_QuadRenderData.VertexData,
                                         static_cast<uint32_t>(reinterpret_cast<uint8_t*>(g_QuadRenderData.LastVertex) -
                                             reinterpret_cast<uint8_t*>(g_QuadRenderData.VertexData)));

        for (uint32_t i = 0; i < g_QuadRenderData.LastTextureSlot; i++)
            g_QuadRenderData.Textures[i]->Bind(i);
        
        m_CommandQueue->Submit<DrawElementsCommand>(g_QuadRenderData.VertexArray, g_QuadRenderData.IndexCount);

        // TODO: Move this to other thread
        while (!m_CommandQueue->IsEmpty()) m_CommandQueue->ExecuteNext();
    }

    void Renderer2D::NextBatch()
    {
        Flush();
        StartBatch();
    }

    void Renderer2D::Begin()
    {
        m_TextureShader->Bind();
        m_TextureShader->SetUniformMat4("u_ProjectionViewMatrix", m_RenderData.RenderCamera.ProjectionViewMatrix());
        m_TextureShader->SetUniformIntArray("u_TextureSlots", g_TextureSlots, QuadRenderData::MaxQuads);
        StartBatch();
    }

    void Renderer2D::StartBatch()
    {
        g_QuadRenderData.LastVertex = g_QuadRenderData.VertexData;
        g_QuadRenderData.QuadCount = 0;
        g_QuadRenderData.IndexCount = 0;
        g_QuadRenderData.LastTextureSlot = 1;
    }

    void Renderer2D::End()
    {
        Flush();
    }

    void Renderer2D::ClearScreen(const glm::vec4 clearColor)
    {
        m_CommandQueue->Submit<SetClearColorCommand>(clearColor);
        m_CommandQueue->Submit<ClearCommand>();
    }

    void Renderer2D::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        m_CommandQueue->Submit<SetViewPortCommand>(x, y, width, height);
    }

    void Renderer2D::SetBlendingMode(const BlendingMode blendingMode)
    {
        m_CommandQueue->Submit<SetBlendingModeCommand>(blendingMode);
    }

    void Renderer2D::SetDepthTestEnabled(const bool bEnabled)
    {
        m_CommandQueue->Submit<SetDepthTestEnabledCommand>(bEnabled);
    }

    void Renderer2D::SetBlendingEnabled(const bool bEnabled)
    {
        m_CommandQueue->Submit<SetBlendingEnabledCommand>(bEnabled);
    }
}
