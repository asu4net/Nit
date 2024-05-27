#include "NitPCH.h"
#include "Core\Engine.h"
#include "Input/KeyCodes.h"
#include "InputSystem.h"

namespace Nit::InputSystem::Keyboard
{
    bool lastKeyboardState[NUM_KEYS];

    const String KeyNames[NUM_KEYS] = 
    {   
        KeyboardKeyNames::Key_Space,
        KeyboardKeyNames::Key_Apostrophe,
        KeyboardKeyNames::Key_Coma,
        KeyboardKeyNames::Key_Minus,
        KeyboardKeyNames::Key_Period,
        KeyboardKeyNames::Key_Slash,
        KeyboardKeyNames::Key_Num0,
        KeyboardKeyNames::Key_Num1,
        KeyboardKeyNames::Key_Num2,
        KeyboardKeyNames::Key_Num3,
        KeyboardKeyNames::Key_Num4,
        KeyboardKeyNames::Key_Num5,
        KeyboardKeyNames::Key_Num6,
        KeyboardKeyNames::Key_Num7,
        KeyboardKeyNames::Key_Num8,
        KeyboardKeyNames::Key_Num9,
        KeyboardKeyNames::Key_Semicolon,
        KeyboardKeyNames::Key_Equal,
        KeyboardKeyNames::Key_A,
        KeyboardKeyNames::Key_B,
        KeyboardKeyNames::Key_C,
        KeyboardKeyNames::Key_D,
        KeyboardKeyNames::Key_E,
        KeyboardKeyNames::Key_F,
        KeyboardKeyNames::Key_G,
        KeyboardKeyNames::Key_H,
        KeyboardKeyNames::Key_I,
        KeyboardKeyNames::Key_J,
        KeyboardKeyNames::Key_K,
        KeyboardKeyNames::Key_L,
        KeyboardKeyNames::Key_M,
        KeyboardKeyNames::Key_N,
        KeyboardKeyNames::Key_O,
        KeyboardKeyNames::Key_P,
        KeyboardKeyNames::Key_Q,
        KeyboardKeyNames::Key_R,
        KeyboardKeyNames::Key_S,
        KeyboardKeyNames::Key_T,
        KeyboardKeyNames::Key_U,
        KeyboardKeyNames::Key_V,
        KeyboardKeyNames::Key_W,
        KeyboardKeyNames::Key_X,
        KeyboardKeyNames::Key_Y,
        KeyboardKeyNames::Key_Z,
        KeyboardKeyNames::Key_LeftBracket,
        KeyboardKeyNames::Key_BackSlash,
        KeyboardKeyNames::Key_RightBracket,
        KeyboardKeyNames::Key_GraveAccent,
        KeyboardKeyNames::Key_World1,
        KeyboardKeyNames::Key_World2,
        KeyboardKeyNames::Key_Escape,
        KeyboardKeyNames::Key_Enter,
        KeyboardKeyNames::Key_Tab,
        KeyboardKeyNames::Key_BackSpace,
        KeyboardKeyNames::Key_Insert,
        KeyboardKeyNames::Key_Delete,
        KeyboardKeyNames::Key_Right,
        KeyboardKeyNames::Key_Left,
        KeyboardKeyNames::Key_Down,
        KeyboardKeyNames::Key_Up,
        KeyboardKeyNames::Key_PageUp,
        KeyboardKeyNames::Key_PageDown,
        KeyboardKeyNames::Key_Home,
        KeyboardKeyNames::Key_End,
        KeyboardKeyNames::Key_CapsLock,
        KeyboardKeyNames::Key_ScrollLock,
        KeyboardKeyNames::Key_NumLock,
        KeyboardKeyNames::Key_PrintScreen,
        KeyboardKeyNames::Key_Pause,
        KeyboardKeyNames::Key_F1,
        KeyboardKeyNames::Key_F2,
        KeyboardKeyNames::Key_F3,
        KeyboardKeyNames::Key_F4,
        KeyboardKeyNames::Key_F5,
        KeyboardKeyNames::Key_F6,
        KeyboardKeyNames::Key_F7,
        KeyboardKeyNames::Key_F8,
        KeyboardKeyNames::Key_F9,
        KeyboardKeyNames::Key_F10,
        KeyboardKeyNames::Key_F11,
        KeyboardKeyNames::Key_F12,
        KeyboardKeyNames::Key_F13,
        KeyboardKeyNames::Key_F14,
        KeyboardKeyNames::Key_F15,
        KeyboardKeyNames::Key_F16,
        KeyboardKeyNames::Key_F17,
        KeyboardKeyNames::Key_F18,
        KeyboardKeyNames::Key_F19,
        KeyboardKeyNames::Key_F20,
        KeyboardKeyNames::Key_F21,
        KeyboardKeyNames::Key_F22,
        KeyboardKeyNames::Key_F23,
        KeyboardKeyNames::Key_F24,
        KeyboardKeyNames::Key_F25,
        KeyboardKeyNames::Key_KP0,
        KeyboardKeyNames::Key_KP1,
        KeyboardKeyNames::Key_KP2,
        KeyboardKeyNames::Key_KP3,
        KeyboardKeyNames::Key_KP4,
        KeyboardKeyNames::Key_KP5,
        KeyboardKeyNames::Key_KP6,
        KeyboardKeyNames::Key_KP7,
        KeyboardKeyNames::Key_KP8,
        KeyboardKeyNames::Key_KP9,
        KeyboardKeyNames::Key_KPDecimal,
        KeyboardKeyNames::Key_KPDivide,
        KeyboardKeyNames::Key_KPMultiply,
        KeyboardKeyNames::Key_KPSubstract,
        KeyboardKeyNames::Key_KPAdd,
        KeyboardKeyNames::Key_KPEnter,
        KeyboardKeyNames::Key_KPEqual,
        KeyboardKeyNames::Key_LeftShift,
        KeyboardKeyNames::Key_LeftControl,
        KeyboardKeyNames::Key_LeftAlt,
        KeyboardKeyNames::Key_LeftSuper,
        KeyboardKeyNames::Key_RightShift,
        KeyboardKeyNames::Key_RightControl,
        KeyboardKeyNames::Key_RightAlt,
        KeyboardKeyNames::Key_RightSuper,
        KeyboardKeyNames::Key_Menu
    };

    const uint32_t KeyCodes[NUM_KEYS] =
    {
        Key_Space,
        Key_Apostrophe,
        Key_Coma,
        Key_Minus,
        Key_Period,
        Key_Slash,
        Key_Num0,
        Key_Num1,
        Key_Num2,
        Key_Num3,
        Key_Num4,
        Key_Num5,
        Key_Num6,
        Key_Num7,
        Key_Num8,
        Key_Num9,
        Key_Semicolon,
        Key_Equal,
        Key_A,
        Key_B,
        Key_C,
        Key_D,
        Key_E,
        Key_F,
        Key_G,
        Key_H,
        Key_I,
        Key_J,
        Key_K,
        Key_L,
        Key_M,
        Key_N,
        Key_O,
        Key_P,
        Key_Q,
        Key_R,
        Key_S,
        Key_T,
        Key_U,
        Key_V,
        Key_W,
        Key_X,
        Key_Y,
        Key_Z,
        Key_LeftBracket,
        Key_BackSlash,
        Key_RightBracket,
        Key_GraveAccent,
        Key_World1,
        Key_World2,
        Key_Escape,
        Key_Enter,
        Key_Tab,
        Key_BackSpace,
        Key_Insert,
        Key_Delete,
        Key_Right,
        Key_Left,
        Key_Down,
        Key_Up,
        Key_PageUp,
        Key_PageDown,
        Key_Home,
        Key_End,
        Key_CapsLock,
        Key_ScrollLock,
        Key_NumLock,
        Key_PrintScreen,
        Key_Pause,
        Key_F1,
        Key_F2,
        Key_F3,
        Key_F4,
        Key_F5,
        Key_F6,
        Key_F7,
        Key_F8,
        Key_F9,
        Key_F10,
        Key_F11,
        Key_F12,
        Key_F13,
        Key_F14,
        Key_F15,
        Key_F16,
        Key_F17,
        Key_F18,
        Key_F19,
        Key_F20,
        Key_F21,
        Key_F22,
        Key_F23,
        Key_F24,
        Key_F25,
        Key_KP0,
        Key_KP1,
        Key_KP2,
        Key_KP3,
        Key_KP4,
        Key_KP5,
        Key_KP6,
        Key_KP7,
        Key_KP8,
        Key_KP9,
        Key_KPDecimal,
        Key_KPDivide,
        Key_KPMultiply,
        Key_KPSubstract,
        Key_KPAdd,
        Key_KPEnter,
        Key_KPEqual,
        Key_LeftShift,
        Key_LeftControl,
        Key_LeftAlt,
        Key_LeftSuper,
        Key_RightShift,
        Key_RightControl,
        Key_RightAlt,
        Key_RightSuper,
        Key_Menu
    };


    void OnUpdate();
    void OnDestroy();

    void Register()
    {
        Engine::CreateSystem("KeyboardInputSystem", InputExecutionOrder, ExecutionContext::Always);
        Engine::SetSystemCallback(SystemStage::Update, &OnUpdate);
        Engine::SetSystemCallback(SystemStage::Destroy, &OnDestroy);
    }

    void OnUpdate()
    {
        // For each button check against the previous state and send the correct message if any
        for (int i = 0; i < NUM_KEYS; ++i)
        {
            bool bIsKeyPressed = Input::IsKeyPressed(KeyCodes[i]);
            if (bIsKeyPressed != lastKeyboardState[i])
            {
                if (bIsKeyPressed)
                {
                    InputSystem::OnControllerButtonPressed(KeyNames[i], 0, false);
                }
                else
                {
                    InputSystem::OnControllerButtonReleased(KeyNames[i], 0, false);
                }
            }
            else if (bIsKeyPressed)
            {
                InputSystem::OnControllerButtonPressed(KeyNames[i], 0, true);
            }

            // Update the state for next time
            lastKeyboardState[i] = bIsKeyPressed;
        }
    }

    void OnDestroy()
    {

    }

}
