#pragma once

namespace Nit
{
    class ComponentPanel
    {
    public:
        ComponentPanel(class Editor* editor);
        void Draw();

    private:
        Editor* m_Editor;
    }; 
}
