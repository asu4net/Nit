#pragma once

namespace Nit
{
    class InputAction;

    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode keyCode);
        static bool IsKeyPressed(uint32_t key);

        static void SetEnabled(bool bConsumed);
        static bool IsEnabled();

        static bool IsMouseButtonPressed(int button);
        static Vector2 GetMousePosition();

        static void Init(const UniquePtr<Window>& window);

        static WeakPtr<InputAction> RegisterInputAction(KeyCode keyCode);
        static bool UnregisterInputAction(WeakPtr<InputAction> inputAction);

        static void UpdateInputActions();

    private:
        inline static bool m_IsEnabled = true;

        //TODO: Make this an input system
        inline static DynamicArray<SharedPtr<InputAction>> m_InputActions;
    };
}