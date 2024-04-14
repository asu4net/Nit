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
        EGraphicsAPI API = EGraphicsAPI::OpenGL;
        CWindow::Configuration WindowConfiguration;
    };
}

namespace Nit::Engine
{
    using RawFunctionPtr = void(*)();

    CWindow&           GetWindow();
    bool              HasWindow();

    void PushSystemID(const TString& name);
    void PopSystemID();
    void CreateSystem(const TString& name, uint32_t executionOrder = 0, ExecutionContext context = ExecutionContext::Runtime, bool startDisabled = false);
    void SetSystemCallback(SystemStage stage, RawFunctionPtr callback);
    void DestroySystem(const TString& name);
    void SetSystemEnabled(const TString& name, bool bEnabled = true);

    void Play();
    void Pause();
    void Stop();

    bool IsPaused();
    bool IsStopped();

    EGraphicsAPI GetGraphicsAPI();

    void SetScreenWidth(uint32_t width);
    void SetScreenHeight(uint32_t height);

    uint32_t GetScreenWidth();
    uint32_t GetScreenHeight();

    void SetScreenSize(const CVector2& screenSize);
    CVector2 GetScreenSize();

    float GetScreenAspect();

    void SetUseWindowScreenSize(bool bUse);
    bool IsUsingWindowScreenSize();

    void Init(const InitConfig& config = {});
    void Run(const RunConfig& config = {});

    void Shutdown();
    void Finish();
}