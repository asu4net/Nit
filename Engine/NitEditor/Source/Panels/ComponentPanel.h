#pragma once

namespace Nit
{
    class ComponentPanel
    {
    public:
        ComponentPanel(class Editor* editor);
        void Draw();

        void DrawComponent(const class Actor& selectedActor, const rttr::type& type);
        
    private:
        Editor* m_Editor;
    }; 
}
