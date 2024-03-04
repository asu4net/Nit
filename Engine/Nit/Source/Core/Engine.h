#pragma once
#include "Window/Window.h"
#include "Entity/World.h"

namespace Nit
{
    struct RunConfig
    {
        bool bStartStopped = false;
    };

    struct InitConfig
    {
        GraphicsAPI API = GraphicsAPI::OpenGL;
        Window::Configuration WindowConfiguration;
    };
}

namespace Nit::Engine
{
    using FunctionPtr = void(*)();

    Window&           GetWindow();
    bool              HasWindow();

    void PushSystemID(const String& name);
    void PopSystemID();
    void CreateSystem(const String& name, uint32_t executionOrder = 0, ExecutionContext context = ExecutionContext::Runtime, bool startDisabled = false);
    void SetSystemCallback(SystemStage stage, FunctionPtr callback);
    void DestroySystem(const String& name);
    void SetSystemEnabled(const String& name, bool bEnabled = true);

    void Play();
    void Pause();
    void Stop();

    bool IsPaused();
    bool IsStopped();

    GraphicsAPI GetGraphicsAPI();

    void SetScreenWidth(uint32_t width);
    void SetScreenHeight(uint32_t height);

    uint32_t GetScreenWidth();
    uint32_t GetScreenHeight();

    void SetScreenSize(const Vector2& screenSize);
    Vector2 GetScreenSize();

    float GetScreenAspect();

    void SetUseWindowScreenSize(bool bUse);
    bool IsUsingWindowScreenSize();

    void Init(const InitConfig& config = {});
    void Run(const RunConfig& config = {});

    void Shutdown();
    void Finish();
}