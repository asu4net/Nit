#pragma once
#include <memory>
#include <string>
#include "imgui.h"
#include "Core/Delegates/MulticastDelegate.h"

namespace Nit
{
    DECLARE_EVENT(class ImGuiWidget, FWidgetCreateEvent)
    DECLARE_EVENT(class ImGuiWidget, FWidgetUpdateEvent)
    DECLARE_EVENT(class ImGuiWidget, FWidgetDestroyEvent)
    
    class ImGuiWidget
    {
    public:
        ImGuiWindowFlags Flags;
        bool* Opened;
        bool Enabled;
        
        ImGuiWidget(const std::string& name = "ImGuiWidget", bool* opened = nullptr, ImGuiWindowFlags flags = 0);
        
        virtual ~ImGuiWidget() = default; 
        
        const std::string& GetName() const { return m_Name; }
        
        Delegate<void(ImGuiWidget*)> GetBeginDelegate() const { return m_BeginDelegate; }
        Delegate<void()> GetEndDelegate() const { return m_EndDelegate; }

        void SetBeginDelegate(const Delegate<void(ImGuiWidget*)>& beginDelegate) { m_BeginDelegate = beginDelegate; }
        void SetEndDelegate(const Delegate<void()>& endDelegate) { m_EndDelegate = endDelegate; }
        
        void ClearBeginEndDelegates();
        
        FWidgetCreateEvent& CreateEvent() { return m_WidgetCreateEvent; }
        FWidgetUpdateEvent& UpdateEvent() { return m_WidgetUpdateEvent; }
        FWidgetDestroyEvent& DestroyEvent() { return m_WidgetDestroyEvent; }

        void Create();
        void Update();
        void Destroy();
        
        template<typename T, typename ...TArgs>
        std::shared_ptr<T> PushWidget(TArgs&& ...args)
        {
            std::shared_ptr<T> widget = std::make_shared<T>(std::forward<TArgs>(args)...);
            m_ChildWidgets.push_back(widget);
            widget->Create();
            return widget;
        }
        
    protected:
        const std::string m_Name;
        
        virtual void OnCreate();
        virtual void OnUpdate();
        virtual void OnDestroy();
    
    private:
        Delegate<void(ImGuiWidget*)> m_BeginDelegate;
        Delegate<void()> m_EndDelegate;
        FWidgetCreateEvent m_WidgetCreateEvent;
        FWidgetUpdateEvent m_WidgetUpdateEvent;
        FWidgetDestroyEvent m_WidgetDestroyEvent;
        std::vector<std::shared_ptr<ImGuiWidget>> m_ChildWidgets;
    };
}