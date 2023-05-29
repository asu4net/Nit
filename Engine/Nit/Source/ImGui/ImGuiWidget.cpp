#ifdef NIT_IMGUI
#include "ImGuiWidget.h"

namespace Nit
{
    ImGuiWidget::ImGuiWidget(const std::string& name, bool* opened, const ImGuiWindowFlags flags)
        : m_Name(name)
        , m_bIsEnabled(true)
        , m_Opened(opened)
        , m_Flags(flags)
    {
    }

    void ImGuiWidget::Start()
    {
        OnStart();
        m_StartEvent.Broadcast();
        for (const auto& child : m_ChildWidgets)
            child->Start();
    }

    void ImGuiWidget::BeginUpdate()
    {
        if (!IsEnabled()) return;
        
        OnBeginUpdate();
        m_BeginUpdateEvent.Broadcast();
        for (const auto& child : m_ChildWidgets)
            child->BeginUpdate();
    }

    void ImGuiWidget::Update()
    {
        if (!IsEnabled()) return;

        OnUpdate();
        m_UpdateEvent.Broadcast();
        for (const auto& child : m_ChildWidgets)
            child->Update();
    }
    
    void ImGuiWidget::EndUpdate()
    {
        if (!IsEnabled()) return;

        OnEndUpdate();
        m_EndUpdateEvent.Broadcast();
        for (const auto& child : m_ChildWidgets)
            child->EndUpdate();
    }

    void ImGuiWidget::Finish()
    {
        OnFinish();
        m_FinishEvent.Broadcast();
        for (const auto& child : m_ChildWidgets)
            child->Finish();
    }
}
#endif