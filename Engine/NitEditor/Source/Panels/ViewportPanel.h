#pragma once

namespace Nit
{
    class FrameBuffer;
    
    class ViewportPanel
    {
    public:
        ViewportPanel();
        void Draw();

    private:
        Vec2 m_ViewportSize;
    };
}