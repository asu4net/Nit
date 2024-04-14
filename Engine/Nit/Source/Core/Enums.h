#pragma once

namespace Nit
{
    enum class EGraphicsAPI
    {
        None, OpenGL
    };

    enum class EBlendingMode
    {
        Alpha,
        Solid,
        Add,
        Multiply
    };

    enum class ExecutionContext : uint8_t
    {
          Invalid = 0
        , Runtime
        , Editor
        , Always
    };

    enum class SystemStage : int32_t
    {
        Invalid = -1,
        Create,
        Start,
        Update,
        LateUpdate,
        FixedUpdate,
        PreDrawPrimitives,
        PostDrawPrimitives,
        Finish,
        Destroy,
        Count
    };

    enum EngineSystemsExecutionOrder : uint32_t
    {
        EditorExecutionOrder    = 10000,
        FamilyExecutionOrder    = 20000,
        InputExecutionOrder     = 30000,
        AnimationExecutionOrder = 40000,
        CameraExecutionOrder    = 50000,
        PhysicsExecutionOrder   = 60000,
        SpriteExecutionOrder    = 70000,
        MaterialExecutionOrder  = 80000,
        AudioExecutionOrder     = 90000
    };

    enum class EMinificationFilter { Linear, Nearest };
    enum class EMagnificationFilter { Linear, Nearest };
    enum class ETextureWrapMode { Repeat, ClampToEdge };
    enum class ETextureCoordinate { U, V };

    namespace EnumsRegistration
    {
        RTTR_REGISTRATION
        {
            using namespace Nit;
            using namespace rttr;

            registration::enumeration<EMinificationFilter>("TextureMinFilter")
            (
                value("Linear", EMinificationFilter::Linear),
                value("Nearest", EMinificationFilter::Nearest)
            );
            registration::enumeration<EMagnificationFilter>("TextureMagFilter")
            (
                value("Linear", EMagnificationFilter::Linear),
                value("Nearest", EMagnificationFilter::Nearest)
            );
            registration::enumeration<ETextureWrapMode>("TextureWrapMode")
            (
                value("Repeat", ETextureWrapMode::Repeat),
                value("ClampToEdge", ETextureWrapMode::ClampToEdge)
            );
        }
    }
}