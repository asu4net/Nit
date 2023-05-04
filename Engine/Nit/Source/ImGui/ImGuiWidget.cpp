#ifdef NIT_IMGUI
#include "ImGuiWidget.h"

namespace Nit
{
    ImGuiWidget::ImGuiWidget(const std::string& name, bool* opened, const ImGuiWindowFlags flags)
        : Flags(flags)
        , Opened(opened)
        , Enabled(true)
        , m_Name(name)
    {
        m_BeginDelegate = {[](const ImGuiWidget* widget)
        {
            ImGui::Begin(widget->GetName().c_str(), widget->Opened, widget->Flags);
        }};
        m_EndDelegate = {[&](){ ImGui::End(); }};
    }

    void ImGuiWidget::ClearBeginEndDelegates()
    {
        m_BeginDelegate.Clear();
        m_EndDelegate.Clear();
    }

    void ImGuiWidget::Create()
    {
        OnCreate();
        m_WidgetCreateEvent.Broadcast();
    }

    void ImGuiWidget::Update()
    {
        if (!Enabled) return;

        m_BeginDelegate(this);
        OnUpdate();
        m_WidgetUpdateEvent.Broadcast();
        for (const auto& child : m_ChildWidgets)
            child->Update();
        m_EndDelegate();
    }
    
    void ImGuiWidget::Destroy()
    {
        OnDestroy();
        ClearBeginEndDelegates();
        m_WidgetDestroyEvent.Broadcast();
        m_WidgetUpdateEvent.RemoveAll();
    }

    void ImGuiWidget::OnCreate() {}
    void ImGuiWidget::OnUpdate() {}
    void ImGuiWidget::OnDestroy() {}
}
#endif