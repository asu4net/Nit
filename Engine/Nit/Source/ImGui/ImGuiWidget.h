#pragma once
#ifdef NIT_IMGUI
#include "Core/Delegates/MulticastDelegate.h"

namespace Nit
{
    DECLARE_EVENT(class ImGuiWidget, FWidgetStartEvent)
    DECLARE_EVENT(class ImGuiWidget, FWidgetBeginUpdateEvent)
    DECLARE_EVENT(class ImGuiWidget, FWidgetUpdateEvent)
    DECLARE_EVENT(class ImGuiWidget, FWidgetEndUpdateEvent)
    DECLARE_EVENT(class ImGuiWidget, FWidgetFinishEvent)
    
    class ImGuiWidget
    {
    public:
        ImGuiWidget(const std::string& name = "ImGuiWidget", bool* opened = nullptr, ImGuiWindowFlags flags = 0);
        virtual ~ImGuiWidget() = default; 
        
        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

        bool IsEnabled() const { return m_bIsEnabled; }
        void SetEnabled(const bool bIsEnabled) { m_bIsEnabled = bIsEnabled; }

        bool* GetOpened() const { return m_Opened; }
        void SetOpened(bool* const opened) { m_Opened = opened; }
        
        ImGuiWindowFlags GetFlags() const { return m_Flags; }
        void SetFlags(const ImGuiWindowFlags flags) { m_Flags = flags; }
        
        FWidgetStartEvent&       StartEvent()       { return m_StartEvent; }
        FWidgetBeginUpdateEvent& BeginUpdateEvent() { return m_BeginUpdateEvent; }
        FWidgetUpdateEvent&      UpdateEvent()      { return m_UpdateEvent; }
        FWidgetEndUpdateEvent&   EndUpdateEvent()   { return m_EndUpdateEvent; }
        FWidgetFinishEvent&      FinishEvent()      { return m_FinishEvent; }
        
        void Start();
        void BeginUpdate();
        void Update();
        void EndUpdate();
        void Finish();
        
        template<typename T, typename ...TArgs>
        Shared<T> PushWidget(TArgs&& ...args)
        {
            Shared<T> widget = std::make_shared<T>(std::forward<TArgs>(args)...);
            m_ChildWidgets.push_back(widget);
            return widget;
        }
        
    protected:
        virtual void OnStart()       {}
        virtual void OnBeginUpdate() {}
        virtual void OnUpdate()      {}
        virtual void OnEndUpdate()   {}
        virtual void OnFinish()      {}
        
    private:
        std::string m_Name;
        bool m_bIsEnabled;
        bool* m_Opened;
        ImGuiWindowFlags m_Flags;
        
        std::vector<Shared<ImGuiWidget>> m_ChildWidgets;

        FWidgetStartEvent       m_StartEvent;
        FWidgetBeginUpdateEvent m_BeginUpdateEvent;
        FWidgetUpdateEvent      m_UpdateEvent;
        FWidgetEndUpdateEvent   m_EndUpdateEvent;
        FWidgetFinishEvent      m_FinishEvent;
    };
}
#endif