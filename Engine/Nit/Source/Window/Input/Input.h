#pragma once

namespace Nit 
{
    class Input
    {
    public:
        static bool IsKeyPressed(int key);

        static void SetConsumedByEditor(bool bConsumedByEditor);
        static bool IsConsumedByEditor();
        
        static void Initialize(void* window);
        static bool IsMouseButtonPressed(int button);
        static Vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
        
    private:
        static bool m_bConsumedByEditor;
        static bool m_bInitialized;
        static void* m_Window;
    };
}