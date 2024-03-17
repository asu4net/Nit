#include "Renderer.h"
#include "RendererShader.h"
#include "RendererTexture2D.h"
#include "Asset/RawShaderStrings.h"
#include "RenderCommand.h"
#include "Core/Engine.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Nit::Renderer
{
    using TextureMap   = Map<Id, SharedPtr<RendererTexture2D>>;
    using ShaderMap    = Map<Id, SharedPtr<RendererShader>>;
    using TextureArray = DynamicArray<SharedPtr<RendererTexture2D>>;
    using SlotArray    = DynamicArray<int32_t>;

    constexpr uint32_t            VerticesPerPrimitive = 4;
    constexpr uint32_t            IndicesPerPrimitive  = 6;
    constexpr uint32_t            MaxTextureSlots      = 32;

    SharedPtr<RendererAPI>        API;
    Matrix4                       ProjectionViewMatrix; // quizás esto podría ir por vertex data también
    TextureMap                    Textures;
    ShaderMap                     Shaders;
    SharedPtr<RendererTexture2D>  WhiteTexture;
    SharedPtr<RendererShader>     ErrorShader;
    bool                          bErrorScreenEnabled = false;
    DynamicArray<Primitive2D*>    Primitives;
    TextureArray                  TexturesToBind       = TextureArray(MaxTextureSlots);
    SlotArray                     TextureSlots         = SlotArray(MaxTextureSlots);
    uint32_t                      LastTextureSlot      = 1;
    SharedPtr<RendererShader>     LastShader           = nullptr;
    Primitive2DType               LastPrimitive2D      = Primitive2DType_Default;

    void NextBatch();
    int CalculateTextureSlot(const SharedPtr<RendererTexture2D>& texture);
    SharedPtr<IndexBuffer> CreateQuadIndexBuffer(uint32_t maxPrimitives);

    namespace SpriteBatch
    {
        struct Vertex
        {
            Vector3  Position      = Vector3::Zero;
            Vector3  LocalPosition = Vector3::Zero;
            Color    TintColor     = Color::White;
            Vector2  UVCoords      = Vector2::Zero;
            uint32_t TextureSlot   = 0;
            float    Time          = 0.f;
            int      EntityID      = -1;
        };

        constexpr uint32_t        MaxPrimitives  = 3000;
        constexpr uint32_t        MaxVertices    = MaxPrimitives * VerticesPerPrimitive;
        constexpr uint32_t        MaxIndices     = MaxPrimitives * IndicesPerPrimitive;

        SharedPtr<RendererShader> DefaultShader;
        SharedPtr<VertexArray>    VAO            = nullptr;
        SharedPtr<VertexBuffer>   VBO            = nullptr;
        SharedPtr<IndexBuffer>    IBO            = nullptr;
        
        //Per batch stuff
        uint32_t                  IndexCount     = 0;
        Vertex*                   Vertices       = new Vertex[MaxVertices];
        Vertex*                   LastVertex     = Vertices;

        void Create()
        {
            if (!API)
            {
                NIT_CHECK(false, "Missing sprite batch stuff\n");
                return;
            }

            VAO = VertexArray::Create(API->GetGraphicsAPI());
            VBO = VertexBuffer::Create(API->GetGraphicsAPI(), MaxVertices * sizeof(Vertex));

            VBO->SetLayout({
                {ShaderDataType::Float3, "a_Position"      },
                {ShaderDataType::Float3, "a_LocalPosition" },
                {ShaderDataType::Float4, "a_TintColor"     },
                {ShaderDataType::Float2, "a_UVCoords"      },
                {ShaderDataType::Float , "a_TextureSlot"   },
                {ShaderDataType::Float , "a_Time"          },
                {ShaderDataType::Float,  "a_EntityID"      }
            });

            VAO->AddVertexBuffer(VBO);
            IBO = CreateQuadIndexBuffer(MaxPrimitives);
            VAO->SetIndexBuffer(IBO);
        }

        void Reset()
        {
            LastVertex      = Vertices;
            IndexCount      = 0;
        }

        void SubmitVertices(SpritePrimitive& sprite)
        {
            if (sprite.ShaderID != 0)
            {
                auto shader = GetShader(sprite.ShaderID);
                if (shader && shader != LastShader)
                {
                    NextBatch();
                    shader->Bind();
                    LastShader = shader;
                }
            }
            else if (DefaultShader && DefaultShader != LastShader)
            {
                NextBatch();
                DefaultShader->Bind();
                DefaultShader->SetUniformIntArray("u_TextureSlots", &TextureSlots.front(), MaxTextureSlots);
                LastShader = DefaultShader;
            }

            if (!LastShader)
            {
                NIT_CHECK(false, "Missing shader!");
                return;
            }

            uint32_t textureSlot = 0;

            if (auto texture = GetTexture2D(sprite.TextureID))
            {
                textureSlot = CalculateTextureSlot(texture); //This could trigger NextBatch()
            }

            Array<Vector2, 4> vertexUV = sprite.VertexUVs;
            RenderUtils::FlipQuadVertexUVs(sprite.bFlipX, sprite.bFlipY, vertexUV);

            if (IndexCount + IndicesPerPrimitive >= MaxIndices)
            {
                NextBatch();
            }

            for (uint32_t i = 0; i < VerticesPerPrimitive; i++)
            {
                Vertex vertex;

                Vector3 localVertexPos = sprite.VertexPositions[i];
                vertex.LocalPosition = localVertexPos;
                
                localVertexPos.x *= sprite.Size.x;
                localVertexPos.y *= sprite.Size.y;

                vertex.Position = Vector4(localVertexPos, 1.f) * sprite.Transform * ProjectionViewMatrix;

                vertex.Time = Time::GetApplicationTime();
                vertex.UVCoords = vertexUV[i];
                vertex.UVCoords.x *= sprite.UVScale.y;
                vertex.UVCoords.y *= sprite.UVScale.x;

                vertex.TintColor = sprite.TintColor;
                vertex.EntityID = sprite.EntityID;
                vertex.TextureSlot = textureSlot;

                *LastVertex = vertex;
                LastVertex->TextureSlot = textureSlot;
                LastVertex++;
            }

            IndexCount += IndicesPerPrimitive;
        }

        void SubmitDrawCommand()
        {
            if (const uint32_t vertexDataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(LastVertex) -
                reinterpret_cast<uint8_t*>(Vertices)))
            {
                VBO->SetData(Vertices, vertexDataSize);

                for (uint32_t i = 0; i < LastTextureSlot; i++)
                    TexturesToBind[i]->Bind(i);

                RenderCommandQueue::Submit<DrawElementsCommand>(API, VAO, IndexCount);
            }

            while (!RenderCommandQueue::IsEmpty())
            {
                RenderCommandQueue::ExecuteNext();
            }
        }
    }; // SpriteBatch

    namespace CircleBatch
    {
        struct Vertex
        {
            Vector3  Position      = Vector3::Zero;
            Vector3  LocalPosition = Vector3::Zero;
            Color    TintColor     = Color::White;
            float    Thickness     = .05f;
            float    Fade          = .01f;
            float    Time          =  0.f;
            int      EntityID      = -1;
        };

        constexpr uint32_t        MaxPrimitives = 3000;
        constexpr uint32_t        MaxVertices = MaxPrimitives * VerticesPerPrimitive;
        constexpr uint32_t        MaxIndices = MaxPrimitives * IndicesPerPrimitive;

        SharedPtr<RendererShader> DefaultShader;
        SharedPtr<VertexArray>    VAO = nullptr;
        SharedPtr<VertexBuffer>   VBO = nullptr;
        SharedPtr<IndexBuffer>    IBO = nullptr;

        //Per batch stuff
        uint32_t                  IndexCount = 0;
        Vertex*                   Vertices = new Vertex[MaxVertices];
        Vertex*                   LastVertex = Vertices;

        void Create()
        {
            if (!API)
            {
                NIT_CHECK(false, "Missing sprite batch stuff\n");
                return;
            }

            VAO = VertexArray::Create(API->GetGraphicsAPI());
            VBO = VertexBuffer::Create(API->GetGraphicsAPI(), MaxVertices * sizeof(Vertex));

            VBO->SetLayout({
                {ShaderDataType::Float3, "a_Position"      },
                {ShaderDataType::Float3, "a_LocalPosition" },
                {ShaderDataType::Float4, "a_TintColor"     },
                {ShaderDataType::Float,  "a_Thickness"     },
                {ShaderDataType::Float , "a_Fade"          },
                {ShaderDataType::Float , "a_Time"          },
                {ShaderDataType::Float,  "a_EntityID"      }
            });

            VAO->AddVertexBuffer(VBO);
            IBO = CreateQuadIndexBuffer(MaxPrimitives);
            VAO->SetIndexBuffer(IBO);
        }

        void Reset()
        {
            LastVertex = Vertices;
            IndexCount = 0;
        }

        void SubmitVertices(CirclePrimitive& circle)
        {
            if (circle.ShaderID != 0)
            {
                auto shader = GetShader(circle.ShaderID);
                if (shader && shader != LastShader)
                {
                    NextBatch();
                    shader->Bind();
                    LastShader = shader;
                }
            }
            else if (DefaultShader && DefaultShader != LastShader)
            {
                NextBatch();
                DefaultShader->Bind();
                LastShader = DefaultShader;
            }

            if (!LastShader)
            {
                NIT_CHECK(false, "Missing shader!");
                return;
            }

            if (IndexCount + IndicesPerPrimitive >= MaxIndices)
            {
                NextBatch();
            }

            for (uint32_t i = 0; i < VerticesPerPrimitive; i++)
            {
                Vertex vertex;

                Vector3 localVertexPos = circle.VertexPositions[i];
                vertex.Time = Time::GetApplicationTime();
                vertex.LocalPosition = localVertexPos;
                vertex.Position = Vector4(localVertexPos * circle.Radius * 2, 1.f) * circle.Transform * ProjectionViewMatrix;
                vertex.TintColor = circle.TintColor;
                vertex.Thickness = circle.Thickness;
                vertex.Fade = circle.Fade;
                vertex.EntityID = circle.EntityID;

                *LastVertex = vertex;
                LastVertex++;
            }

            IndexCount += IndicesPerPrimitive;
        }

        void SubmitDrawCommand()
        {
            if (const uint32_t vertexDataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(LastVertex) -
                reinterpret_cast<uint8_t*>(Vertices)))
            {
                VBO->SetData(Vertices, vertexDataSize);

                RenderCommandQueue::Submit<DrawElementsCommand>(API, VAO, IndexCount);
            }

            while (!RenderCommandQueue::IsEmpty())
            {
                RenderCommandQueue::ExecuteNext();
            }
        }
    }; // CircleBatch
                          
    SharedPtr<IndexBuffer> CreateQuadIndexBuffer(uint32_t maxPrimitives)
    {
        const uint32_t maxIndices = maxPrimitives * IndicesPerPrimitive;

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
        auto idxBuffer = IndexBuffer::Create(API->GetGraphicsAPI(), indices, maxIndices);
        delete[] indices;
        return idxBuffer;
    }

    int CalculateTextureSlot(const SharedPtr<RendererTexture2D>& texture)
    {
        int textureSlot = 0;

        if (texture)
        {
            // Search existing texture
            for (uint32_t i = 1; i < LastTextureSlot; i++)
            {
                if (TexturesToBind[i]->GetTextureID() == texture->GetTextureID())
                {
                    textureSlot = i;
                    break;
                }
            }

            // If not exists save the new texture
            if (textureSlot == 0)
            {
                if (LastTextureSlot > MaxTextureSlots)
                {
                    NextBatch();
                }

                TexturesToBind[LastTextureSlot] = texture;
                textureSlot = LastTextureSlot;
                LastTextureSlot++;
            }
        }

        return textureSlot;
    }

    void StartBatch()
    {
        SpriteBatch::Reset();
        CircleBatch::Reset();

        LastTextureSlot = 1;
    }

    void Invalidate()
    {
        SpriteBatch::SubmitDrawCommand();
        CircleBatch::SubmitDrawCommand();
    }

    void NextBatch()
    {
        Invalidate();
        StartBatch();
    }

    void Init(GraphicsAPI api)
    {
        API = RendererAPI::Create(api);
        ErrorShader = RendererShader::Create(api);

        // Create the white texture
        {
            Texture2DSettings settings;
            settings.Width = 1;
            settings.Height = 1;
            settings.Channels = 4;

            WhiteTexture = RendererTexture2D::Create(api);
            constexpr uint32_t whiteTextureData = 0xffffffff;
            WhiteTexture->UploadToGPU(settings, &whiteTextureData);
            TexturesToBind[0] = WhiteTexture;
        }

        // Create the texture slots
        {
            for (uint32_t i = 0; i < MaxTextureSlots; i++)
                TextureSlots[i] = i;
        }

        SpriteBatch::Create();
        CircleBatch::Create();

        // Default settings
        RenderCommandQueue::Submit<SetBlendingEnabledCommand>(API, true);
        RenderCommandQueue::Submit<SetBlendingModeCommand>(API, BlendingMode::Alpha);
        RenderCommandQueue::Submit<SetClearColorCommand>(API, Color::DarkGrey);

        ErrorShader->Compile(g_ErrorVertexSource, g_ErrorFragmentSource);
        
        NIT_LOG_TRACE("Renderer initialized!\n");
    }

    void SetErrorScreenEnabled(bool bEnabled)
    {
        bErrorScreenEnabled = bEnabled;
    }

    SharedPtr<RendererAPI> GetAPI()
    {
        return API;
    }

    void DrawPrimitives()
    {
        RenderCommandQueue::Submit<SetClearColorCommand>(API, bErrorScreenEnabled ? Color::Black : Color::DarkGrey);
        RenderCommandQueue::Submit<ClearCommand>(API);

        if (bErrorScreenEnabled)
        {
            while (!RenderCommandQueue::IsEmpty())
            {
                RenderCommandQueue::ExecuteNext();
            }
            return;
        }

        StartBatch();

        std::sort(Primitives.begin(), Primitives.end(), [](const Primitive2D* a, const Primitive2D* b) 
        { 
            return a->SortingLayer < b->SortingLayer;
        });

        for (Primitive2D* primitive : Primitives)
        {
            if (!primitive->bIsVisible)
                continue;

            if (LastPrimitive2D != primitive->GetType())
            {
                NextBatch();
            }

            if (primitive->GetType() == Primitive2DType_Sprite)
            {
                SpriteBatch::SubmitVertices(*static_cast<SpritePrimitive*>(primitive));
                LastPrimitive2D = Primitive2DType_Sprite;
                continue;
            }

            if (primitive->GetType() == Primitive2DType_Circle)
            {
                CircleBatch::SubmitVertices(*static_cast<CirclePrimitive*>(primitive));
                LastPrimitive2D = Primitive2DType_Circle;
                continue;
            }
        }

        Invalidate();
    }

    void PushPrimitive(Primitive2D* primitive)
    {
        if (!primitive)
            return;
        Primitives.push_back(primitive);
    }

    void PopPrimitive(Primitive2D* primitive)
    {
        if (!primitive)
            return;
        auto it = std::find(Primitives.begin(), Primitives.end(), primitive);
        if (it == Primitives.end())
            return;
        Primitives.erase(it);
    }

    void DestroyPrimitive(Primitive2D* primitive)
    {
        if (!primitive)
            return;
        PopPrimitive(primitive);
        delete primitive;
    }

    Id CreateTexture2D(const Texture2DSettings& settings, const void* data)
    {
        Id id;
        SharedPtr<RendererTexture2D> texture = RendererTexture2D::Create(API->GetGraphicsAPI());
        texture->UploadToGPU(settings, data);
        Textures[id] = texture;
        return id;
    }

    SharedPtr<RendererTexture2D> GetTexture2D(Id id)
    {
        if (!Textures.count(id))
            return nullptr;
        return Textures[id];
    }

    void DestroyTexture2D(Id id)
    {
        NIT_CHECK(Textures.count(id), "Invalid id!");
        SharedPtr<RendererTexture2D> texture = Textures[id];
        Textures.erase(id);
    }
    
    Id CreateShader(const char* vertexSource, const char* fragmentSource)
    {
        Id id;
        SharedPtr<RendererShader> shader = RendererShader::Create(API->GetGraphicsAPI());
        shader->Compile(vertexSource, fragmentSource);
        Shaders[id] = shader;
        return id;
    }

    SharedPtr<RendererShader> GetShader(Id id)
    {
        NIT_CHECK(Shaders.count(id), "Invalid id!");
        return Shaders[id];
    }

    void DestroyShader(Id id)
    {
        NIT_CHECK(Shaders.count(id), "Invalid id!");
        SharedPtr<RendererShader> texture = Shaders[id];
        Shaders.erase(id);
    }

    void SetSpriteShader(const SharedPtr<RendererShader> spriteShader)
    {
        SpriteBatch::DefaultShader = spriteShader;
    }

    void SetCircleShader(const SharedPtr<RendererShader> circleShader)
    {
        CircleBatch::DefaultShader = circleShader;
    }

    void SetProjectionViewMatrix(const Matrix4& matrix)
    {
        ProjectionViewMatrix = matrix;
    }
}