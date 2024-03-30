#include "EditorSystem.h"
#include <imgui.h>
#include "ImGuizmo.h"
#include "Core\Engine.h"
#include "ImGui\ImGuiRenderer.h"
#include "ImGui\ImGuiUtils.h"
#include "GridSystem.h"
#include "EditorCameraSystem.h"
#include "Windows\ViewportWindow.h"
#include "Windows\StatsWindow.h"
#include "Render\Framebuffer.h"
#include "Windows\SceneWindow.h"
#include "Windows\HierarchyWindow.h"
#include "Windows\PropertiesWindow.h"
#include "ContentSystem.h"
#include "GizmoSystem.h"
#include "Windows\SpritePackerWindow.h"


namespace Nit::EditorSystem
{
    const String ID = "EditorSystem";
    SharedPtr<Framebuffer> TargetFramebuffer;
    Entity SelectedEntity;

    void SetSelectedEntity(Entity entity) 
    { 
        SelectedEntity = entity; 
    }
    
    Entity GetSelectedEntity() 
    { 
        return SelectedEntity; 
    }


    String GetID()
    {
        return ID;
    }

    void CreateFramebuffer()
    {
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        fbSpec.Width = Engine::GetScreenWidth();
        fbSpec.Height = Engine::GetScreenHeight();
        TargetFramebuffer = Framebuffer::Create(Engine::GetGraphicsAPI(), fbSpec);
    }

    void OnCreate()
    {
        Engine::Stop();
        ImGuiRenderer::Init();

        CreateFramebuffer();
        Engine::SetUseWindowScreenSize(false);
    }

    void OnUpdate()
    {
        if (ImGui::IsKeyPressed(ImGuiKey_Delete) && SelectedEntity.IsValid())
        {
            ImGuizmo::Enable(false);
            World::DestroyEntity(SelectedEntity);
            SelectedEntity.Reset();
            ImGuizmo::Enable(true);
        }
    }
    
    void OnPreDrawPrimitives()
    {
        ImGuiRenderer::Begin();

        static bool bShowViewport     = true;
        static bool bShowScene        = true;
        static bool bShowHierarchy    = true;
        static bool bShowProperties   = true;
        static bool bShowSpritePacker = false;
        static bool bShowStats        = false;
        static bool bShowImGuiDemo    = false;

        //Menu bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Windows"))
            {
                ImGui::MenuItem("Viewport",      nullptr, &bShowViewport);
                ImGui::MenuItem("Scene",         nullptr, &bShowScene);
                ImGui::MenuItem("Hierarchy",     nullptr, &bShowHierarchy);
                ImGui::MenuItem("Properties",    nullptr, &bShowProperties);
                ImGui::MenuItem("Sprite packer", nullptr, &bShowSpritePacker);
                ImGui::MenuItem("Stats",         nullptr, &bShowStats);
                ImGui::MenuItem("ImGui demo",    nullptr, &bShowImGuiDemo);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (bShowViewport)     ViewportWindow::Show(&bShowViewport, TargetFramebuffer.get());
        if (bShowScene)        SceneWindow::Show(&bShowScene);
        if (bShowHierarchy)    HierarchyWindow::Show(&bShowHierarchy);
        if (bShowProperties)   PropertiesWindow::Show(&bShowProperties);
        if (bShowSpritePacker) SpritePackerWindow::Show(&bShowSpritePacker);
        if (bShowStats)        StatsWindow::Show(&bShowStats);
        if (bShowImGuiDemo)    ImGui::ShowDemoWindow (&bShowImGuiDemo);
    }

    void OnPostDrawPrimitives()
    {
        if (TargetFramebuffer)
        {
            TargetFramebuffer->Unbind();
        }
        ImGuiRenderer::End();
    }

    void OnDestroy()
    {
        ImGuiRenderer::Finish();
    }

    void Register()
    {
        Engine::CreateSystem(ID, EditorExecutionOrder, ExecutionContext::Always);
        Engine::SetSystemCallback(SystemStage::Create, OnCreate);
        Engine::SetSystemCallback(SystemStage::Update, OnUpdate);
        Engine::SetSystemCallback(SystemStage::PreDrawPrimitives, OnPreDrawPrimitives);
        Engine::SetSystemCallback(SystemStage::PostDrawPrimitives, OnPostDrawPrimitives);
        Engine::SetSystemCallback(SystemStage::Destroy, OnDestroy);
        
        ContentSystem::Register();
        GridSystem::Register();
        EditorCameraSystem::Register();
        GizmoSystem::Register();
    }
}