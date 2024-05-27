#pragma once

namespace Nit
{
    enum class GraphicsAPI
    {
        None, OpenGL
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

    enum class MinificationFilter { Linear, Nearest };
    enum class MagnificationFilter { Linear, Nearest };
    enum class TextureWrapMode { Repeat, ClampToEdge };
    enum class TextureCoordinate { U, V };

    enum class CameraProjection
    {
        Orthographic,
        Perspective
    };

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
            registration::enumeration<CameraProjection>("CameraProjection")
            (
                value("Orthographic", CameraProjection::Orthographic),
                value("Perspective", CameraProjection::Perspective)
            );
        }
    }
}