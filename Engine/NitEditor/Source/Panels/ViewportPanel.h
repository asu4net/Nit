#pragma once

namespace Nit
{
    class FrameBuffer;
    
    class ViewportPanel
    {
    public:
        enum class GizmoMode { Translate, Rotate, Scale };
        
        ViewportPanel(class Editor* editor);
        void Draw();

    private:
        GizmoMode m_GizmoMode = GizmoMode::Translate;
        bool m_GizmosLocal = true;
        float m_Snap = 0;
        Vec2 m_ViewportSize;
        Editor* m_Editor;

        void DrawViewport();
        void DrawGizmos();
    };
}