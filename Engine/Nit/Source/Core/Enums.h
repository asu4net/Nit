#pragma once

namespace Nit
{
    enum class GraphicsAPI
    {
        None, OpenGL
    };

    enum class Flip
    {
        None, X, Y, Both
    };

    enum class BlendingMode
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
        InputExecutionOrder     = 1000,
        AnimationExecutionOrder = 2000,
        CameraExecutionOrder    = 3000,
        SpriteExecutionOrder    = 4000
    };

    enum class MinificationFilter { Linear, Nearest };
    enum class MagnificationFilter { Linear, Nearest };
    enum class TextureWrapMode { Repeat, ClampToEdge };
    enum class TextureCoordinate { U, V };
    enum class SpriteSource { Default, SubSprite };

    namespace EnumsRegistration
    {
        RTTR_REGISTRATION
        {
            using namespace Nit;
            using namespace rttr;

            registration::enumeration<MinificationFilter>("TextureMinFilter")
            (
                value("Linear", MinificationFilter::Linear),
                value("Nearest", MinificationFilter::Nearest)
            );
            registration::enumeration<MagnificationFilter>("TextureMagFilter")
            (
                value("Linear", MagnificationFilter::Linear),
                value("Nearest", MagnificationFilter::Nearest)
            );
            registration::enumeration<TextureWrapMode>("TextureWrapMode")
            (
                value("Repeat", TextureWrapMode::Repeat),
                value("ClampToEdge", TextureWrapMode::ClampToEdge)
            );

            registration::enumeration<SpriteSource>("SpriteSource")
            (
                value("Default", SpriteSource::Default),
                value("SubSprite", SpriteSource::SubSprite)
            );

            registration::enumeration<Flip>("Flip")
            (
                value("None", Flip::None),
                value("X", Flip::X),
                value("Y", Flip::Y),
                value("Both", Flip::Both)
            );
        }
    }
}