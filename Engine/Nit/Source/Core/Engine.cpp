#include "Core/Engine.h"
#include "Asset/Content.h"
#include "Entity/World.h"
#include "Render/Renderer.h"
#include "System/SpriteSystem.h"
#include "System/AnimationSystem.h"
#include "System/CameraSystem.h"
#include "System/ScriptSystem.h"
#include "Component/CameraComponent.h"
#include "Component/ScriptComponent.h"

namespace Nit::Engine
{
    struct System
    {
        String                         Name               = "Invalid";
        uint32_t                       ExecutionOrder     = 0;
        ExecutionContext               Context            = ExecutionContext::Invalid;
        bool                           bIsEnabled         = false;
        bool                           bDestroyRequested  = false;
        Map<SystemStage, FunctionPtr>  Callbacks;
    };

    bool                  bIsInitialized                     = false;
    bool                  bIsRunning                         = false;
    bool                  bIsPaused                          = false;
    bool                  bIsStopped                         = false;
    UniquePtr<Window>     MainWindow                         = nullptr;
    GraphicsAPI           API                                = GraphicsAPI::None;
    uint32_t              ScreenWidth                        = 0;
    uint32_t              ScreenHeight                       = 0;
    Vector2               ScreenSize;
    bool                  bUseWindowScreenSize               = true;
    Entity                FreeLookCameraEntity;              
    Map<String, System*>  SystemMap;                         
    System*               CurrentSystem                      = nullptr;
    bool                  bCreatingSystem                    = false;
    DynamicArray<System*> IterableSystems;
    DynamicArray<System*> PendingToDestroySystems;
    bool                  bInvalidateIterableSystemsRequested = false;

    Window& GetWindow() { return *MainWindow; }
    bool HasWindow() { return MainWindow.get(); }
    
    static System* GetSystem(const String& name)
    {
        NIT_CHECK(SystemMap.count(name), "Missing system!");

        if (!SystemMap.count(name))
        {
            return nullptr;
        }

        System* system = SystemMap[name];

        NIT_CHECK(system, "Null system");
        return system;
    }

    static void InvokeSystemCallback(System* system, SystemStage stage)
    {
        NIT_CHECK(system, "Null system");

        if (!system || !system->Callbacks.count(stage))
            return;

        if (FunctionPtr callback = system->Callbacks[stage])
        {
            callback();
            return;
        }

        NIT_CHECK(false, "Null callback");
    }

    static void InvalidateIterableSystems()
    {
        IterableSystems.clear();

        for (auto& [name, system] : SystemMap)
        {
            if (system->bDestroyRequested)
                continue;

            IterableSystems.push_back(system);
        }

        std::sort(IterableSystems.begin(), IterableSystems.end(), [](System* a, System* b) {
            return a->ExecutionOrder < b->ExecutionOrder;
        });
    }

    static void CleanupPendingToDestroySystems()
    {
        for (System* system : PendingToDestroySystems)
        {
            SystemMap.erase(system->Name);
            delete system;
        }

        PendingToDestroySystems.clear();
    }

    static void InvokeIterableSystemsCallback(SystemStage stage, bool bExcludeDisabledSystems = false)
    {
        for (System* system : IterableSystems)
        {
            NIT_CHECK(system, "Null system");

            if (!system || system->bDestroyRequested || (bExcludeDisabledSystems && !system->bIsEnabled) || 
                (bIsPaused && system->Context == ExecutionContext::Runtime))
                continue;

            InvokeSystemCallback(system, stage);
        }

        if (bInvalidateIterableSystemsRequested)
        {
            InvalidateIterableSystems();
            CleanupPendingToDestroySystems();
            bInvalidateIterableSystemsRequested = false;
        }
    }

    static void ClearAllSystems()
    {
        for (System* system : PendingToDestroySystems)
        {
            DestroySystem(system->Name);
        }

        InvalidateIterableSystems();
        CleanupPendingToDestroySystems();

        for (auto& [name, system] : SystemMap)
        {
            if (!system)
            {
                NIT_CHECK(false, "Null system");
                continue;
            }
            delete system;
        }

        SystemMap.clear();
    }

    void PushSystemID(const String& name)
    {
        PopSystemID();
        CurrentSystem = GetSystem(name);
    }
    
    void PopSystemID()
    {
        CurrentSystem = nullptr;
        bCreatingSystem = false;
    }

    void CreateSystem(const String& name, uint32_t executionOrder, ExecutionContext context, bool startDisabled)
    {
        NIT_CHECK(!SystemMap.count(name), "A system with that name already exists!");

        if (SystemMap.count(name))
        {
            return;
        }

        System* system = new System();
        system->Name = name;
        system->Context = context;
        system->ExecutionOrder = executionOrder;
        system->bIsEnabled = !startDisabled;
        SystemMap[name] = system;

        PushSystemID(name);
        bInvalidateIterableSystemsRequested = false;
        bCreatingSystem = true;
    }

    void SetSystemCallback(SystemStage stage, FunctionPtr callback)
    {
        NIT_CHECK(CurrentSystem, "Not system ID pushed!");

        if (!CurrentSystem)
            return;

        CurrentSystem->Callbacks[stage] = callback;

        if (bCreatingSystem && stage == SystemStage::Create)
        {
            InvokeSystemCallback(CurrentSystem, SystemStage::Create);

            if (bIsStopped || bIsPaused && CurrentSystem->Context == ExecutionContext::Runtime)
                return;

            InvokeSystemCallback(CurrentSystem, SystemStage::Start);
        }
    }

    void DestroySystem(const String& name)
    {
        if (System* system = GetSystem(name))
        {
            if (!system->bDestroyRequested)
            {
                system->bDestroyRequested = true;
                InvokeSystemCallback(system, SystemStage::Finish);
                InvokeSystemCallback(system, SystemStage::Destroy);
            }
            bInvalidateIterableSystemsRequested = true;
        }
    }
    
    void SetSystemEnabled(const String& name, bool bEnabled)
    {
        if (System* system = GetSystem(name))
        {
            if (system->bIsEnabled == bEnabled)
                return;
            
            system->bIsEnabled = bEnabled;
        }
    }

    void Play()  
    { 
        bIsPaused = false; 

        if (bIsStopped)
        {
            bIsStopped = false;
            World::SerializeOpenedScenes();
            InvokeIterableSystemsCallback(SystemStage::Start);
        }
    }

    void Pause() 
    { 
        bIsPaused = true;  
    }

    void Stop()
    {
        bIsStopped = true;
        bIsPaused = true;
        InvokeIterableSystemsCallback(SystemStage::Finish);
        World::ResetOpenedScenes();
    }

    bool IsPaused()
    {
        return bIsPaused;
    }

    bool IsStopped()
    {
        return bIsStopped;
    }


    GraphicsAPI GetGraphicsAPI()
    {
        return API;
    }

    void SetScreenWidth(uint32_t width)
    {
        NIT_CHECK(!bUseWindowScreenSize, "should call SetUseWindowScreenSize");
        if (bUseWindowScreenSize)
            return;
        ScreenWidth = width;
        ScreenSize.x = static_cast<float>(width);
    }

    void SetScreenHeight(uint32_t height)
    {
        NIT_CHECK(!bUseWindowScreenSize, "should call SetUseWindowScreenSize");
        if (bUseWindowScreenSize)
            return;
        ScreenHeight = height;
        ScreenSize.y = static_cast<float>(height);
    }

    uint32_t GetScreenWidth()
    {
        return ScreenWidth;
    }

    uint32_t GetScreenHeight()
    {
        return ScreenHeight;
    }


    void SetScreenSize(const Vector2& screenSize)
    {
        NIT_CHECK(!bUseWindowScreenSize, "should call SetUseWindowScreenSize");
        if (bUseWindowScreenSize)
            return;
        
        ScreenSize = screenSize;
        ScreenWidth = static_cast<uint32_t>(screenSize.x);
        ScreenHeight = static_cast<uint32_t>(screenSize.y);
        Renderer::SetViewport(0, 0, ScreenWidth, ScreenHeight);
    }

    Vector2 GetScreenSize()
    {
        return ScreenSize;
    }

    float GetScreenAspect()
    {
        return ScreenSize.x / ScreenSize.y;
    }

    void SetUseWindowScreenSize(bool bUse)
    {
        bUseWindowScreenSize = bUse;
    }

    bool IsUsingWindowScreenSize()
    {
        return bUseWindowScreenSize;
    }

    static void OnStateInputPressed(const InputActionContext& context)
    {
        if (!context.IsPressed) return;

        if (Input::IsKeyPressed(KeyCode::LeftCtrl)) // play / stop
        {
            if (bIsStopped) Play();
            else Stop();
        }
        else if (Input::IsKeyPressed(KeyCode::LeftAlt)) // play / pause
        {
            if (bIsPaused)  Play();
            else Pause();
        }
    }

    static void SetRawScreenSize(int width, int height)
    {
        if (!bUseWindowScreenSize)
            return;

        ScreenWidth = width;
        ScreenHeight = height;
        ScreenSize.x = static_cast<float>(width);
        ScreenSize.y = static_cast<float>(height);
    }

    void Init(const InitConfig& config)
    {
        NIT_LOG_TRACE("Initializing Engine...\n");
        NIT_CHECK(!bIsInitialized, "Engine already initialized! \n");

        const Window::Configuration windowCfg = config.WindowConfiguration;
        MainWindow = Window::Create(windowCfg);
        SetRawScreenSize(windowCfg.Width, windowCfg.Height);

        Time::Init();
        Input::Init(MainWindow);
        Renderer::Init(config.API); API = config.API;

        // Viewport resizing for renderer
        MainWindow->Events().ResizeEvent.Add([&](int width, int height) { 
            
            Renderer::SetViewport(0, 0, width, height);
            SetRawScreenSize(width, height);
        });
        
        World::Init();

        SpriteSystem::Register();
        AnimationSystem::Register();
        CameraSystem::Register();
        ScriptSystem::Register();
        
        Content::LoadAssets();

        World::OpenDefaultScene();

        Input::RegisterInputAction(KeyCode::P).lock()->OnPerformed().AddRaw(&OnStateInputPressed);

        bIsInitialized = true;

        NIT_LOG_TRACE("Engine initialized!\n");
    }

    void Run(const RunConfig& config)
    {  
        NIT_CHECK(bIsInitialized, "Engine needs to be initialized, call Init() before Run()! \n");
        NIT_CHECK(!bIsRunning, "Engine already running! \n");
        bIsRunning = true;

        bIsPaused = config.bStartStopped;
        bIsStopped = config.bStartStopped;

        InvalidateIterableSystems();
        CleanupPendingToDestroySystems();
        InvokeIterableSystemsCallback(SystemStage::Start);

        NIT_LOG_TRACE("******************************\n");
        NIT_LOG_TRACE("***** MAIN LOOP STARTED ******\n");
        NIT_LOG_TRACE("******************************\n");

        //TODO: Frame by frame mode
        // MAIN LOOP
        while (MainWindow->IsOpened())
        {
            Time::CalculateTimeStep();
            Input::UpdateInputActions();

            InvokeIterableSystemsCallback(SystemStage::Update, true);
            InvokeIterableSystemsCallback(SystemStage::LateUpdate, true);

            uint32_t fixedUpdateCalls = Time::GetFixedUpdateCalls();

            while (fixedUpdateCalls--)
            {
                InvokeIterableSystemsCallback(SystemStage::FixedUpdate, true);
            }

            InvokeIterableSystemsCallback(SystemStage::PreDrawPrimitives, true);
            Renderer::DrawPrimitives();
            InvokeIterableSystemsCallback(SystemStage::PostDrawPrimitives, true);

            MainWindow->Update(); // Poll events and if is using GLFW, swap buffers.
        }

        NIT_LOG_TRACE("******************************\n");
        NIT_LOG_TRACE("***** MAIN LOOP FINISHED *****\n");
        NIT_LOG_TRACE("******************************\n");

        Finish();
    }
    
    void Shutdown()
    {
        MainWindow->Close();
    }

    void Finish()
    {
        if (!bIsInitialized || !bIsRunning)
        {
            return;
        }

        NIT_LOG_TRACE("Finishing engine...\n");
        
        World::Finish();
        ClearAllSystems();
        Content::UnloadAssets();
        MainWindow.reset();

        bIsRunning     = false;
        bIsInitialized = false;

        NIT_LOG_TRACE("Engine finished!\n");
    }
}