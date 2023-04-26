#pragma once

namespace Nit 
{
    class Input
    {
    public:
        static bool IsKeyPressed(int key);

        static void Initialize(void* window);
        static bool IsMouseButtonPressed(int button);
        static glm::vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
        
    private:
        static bool m_Initialized;
        static void* m_Window;
    };
}