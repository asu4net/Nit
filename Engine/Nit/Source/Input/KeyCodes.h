#pragma once
#include "NitPCH.h"

/*! @name Key and button actions
 *  @{ */
 /*! @brief The key or mouse button was released.
  *
  *  The key or mouse button was released.
  *
  *  @ingroup input
  */
#define RELEASE                0
  /*! @brief The key or mouse button was pressed.
   *
   *  The key or mouse button was pressed.
   *
   *  @ingroup input
   */
#define PRESS                  1
   /*! @brief The key was held down until it repeated.
    *
    *  The key was held down until it repeated.
    *
    *  @ingroup input
    */
#define REPEAT                 2
    /*! @} */

    /*! @defgroup hat_state Joystick hat states
     *  @brief Joystick hat states.
     *
     *  See [joystick hat input](@ref joystick_hat) for how these are used.
     *
     *  @ingroup input
     *  @{ */
#define HAT_CENTERED           0
#define HAT_UP                 1
#define HAT_RIGHT              2
#define HAT_DOWN               4
#define HAT_LEFT               8
#define HAT_RIGHT_UP           (HAT_RIGHT | HAT_UP)
#define HAT_RIGHT_DOWN         (HAT_RIGHT | HAT_DOWN)
#define HAT_LEFT_UP            (HAT_LEFT  | HAT_UP)
#define HAT_LEFT_DOWN          (HAT_LEFT  | HAT_DOWN)
     /*! @} */

     /*! @defgroup keys Keyboard keys
      *  @brief Keyboard key IDs.
      *
      *  See [key input](@ref input_key) for how these are used.
      *
      *  These key codes are inspired by the _USB HID Usage Tables v1.12_ (p. 53-60),
      *  but re-arranged to map to 7-bit ASCII for printable keys (function keys are
      *  put in the 256+ range).
      *
      *  The naming of the key codes follow these rules:
      *   - The US keyboard layout is used
      *   - Names of printable alphanumeric characters are used (e.g. "A", "R",
      *     "3", etc.)
      *   - For non-alphanumeric characters, Unicode:ish names are used (e.g.
      *     "COMMA", "LEFT_SQUARE_BRACKET", etc.). Note that some names do not
      *     correspond to the Unicode standard (usually for brevity)
      *   - Keys that lack a clear US mapping are named "WORLD_x"
      *   - For non-printable keys, custom names are used (e.g. "F4",
      *     "BACKSPACE", etc.)
      *
      *  @ingroup input
      *  @{
      */

      /* The unknown key */
#define KEY_UNKNOWN            -1

/* Printable keys */
#define KEY_SPACE              32
#define KEY_APOSTROPHE         39  /* ' */
#define KEY_COMMA              44  /* , */
#define KEY_MINUS              45  /* - */
#define KEY_PERIOD             46  /* . */
#define KEY_SLASH              47  /* / */
#define KEY_0                  48
#define KEY_1                  49
#define KEY_2                  50
#define KEY_3                  51
#define KEY_4                  52
#define KEY_5                  53
#define KEY_6                  54
#define KEY_7                  55
#define KEY_8                  56
#define KEY_9                  57
#define KEY_SEMICOLON          59  /* ; */
#define KEY_EQUAL              61  /* = */
#define KEY_A                  65
#define KEY_B                  66
#define KEY_C                  67
#define KEY_D                  68
#define KEY_E                  69
#define KEY_F                  70
#define KEY_G                  71
#define KEY_H                  72
#define KEY_I                  73
#define KEY_J                  74
#define KEY_K                  75
#define KEY_L                  76
#define KEY_M                  77
#define KEY_N                  78
#define KEY_O                  79
#define KEY_P                  80
#define KEY_Q                  81
#define KEY_R                  82
#define KEY_S                  83
#define KEY_T                  84
#define KEY_U                  85
#define KEY_V                  86
#define KEY_W                  87
#define KEY_X                  88
#define KEY_Y                  89
#define KEY_Z                  90
#define KEY_LEFT_BRACKET       91  /* [ */
#define KEY_BACKSLASH          92  /* \ */
#define KEY_RIGHT_BRACKET      93  /* ] */
#define KEY_GRAVE_ACCENT       96  /* ` */
#define KEY_WORLD_1            161 /* non-US #1 */
#define KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE             256
#define KEY_ENTER              257
#define KEY_TAB                258
#define KEY_BACKSPACE          259
#define KEY_INSERT             260
#define KEY_DELETE             261
#define KEY_RIGHT              262
#define KEY_LEFT               263
#define KEY_DOWN               264
#define KEY_UP                 265
#define KEY_PAGE_UP            266
#define KEY_PAGE_DOWN          267
#define KEY_HOME               268
#define KEY_END                269
#define KEY_CAPS_LOCK          280
#define KEY_SCROLL_LOCK        281
#define KEY_NUM_LOCK           282
#define KEY_PRINT_SCREEN       283
#define KEY_PAUSE              284
#define KEY_F1                 290
#define KEY_F2                 291
#define KEY_F3                 292
#define KEY_F4                 293
#define KEY_F5                 294
#define KEY_F6                 295
#define KEY_F7                 296
#define KEY_F8                 297
#define KEY_F9                 298
#define KEY_F10                299
#define KEY_F11                300
#define KEY_F12                301
#define KEY_F13                302
#define KEY_F14                303
#define KEY_F15                304
#define KEY_F16                305
#define KEY_F17                306
#define KEY_F18                307
#define KEY_F19                308
#define KEY_F20                309
#define KEY_F21                310
#define KEY_F22                311
#define KEY_F23                312
#define KEY_F24                313
#define KEY_F25                314
#define KEY_KP_0               320
#define KEY_KP_1               321
#define KEY_KP_2               322
#define KEY_KP_3               323
#define KEY_KP_4               324
#define KEY_KP_5               325
#define KEY_KP_6               326
#define KEY_KP_7               327
#define KEY_KP_8               328
#define KEY_KP_9               329
#define KEY_KP_DECIMAL         330
#define KEY_KP_DIVIDE          331
#define KEY_KP_MULTIPLY        332
#define KEY_KP_SUBTRACT        333
#define KEY_KP_ADD             334
#define KEY_KP_ENTER           335
#define KEY_KP_EQUAL           336
#define KEY_LEFT_SHIFT         340
#define KEY_LEFT_CONTROL       341
#define KEY_LEFT_ALT           342
#define KEY_LEFT_SUPER         343
#define KEY_RIGHT_SHIFT        344
#define KEY_RIGHT_CONTROL      345
#define KEY_RIGHT_ALT          346
#define KEY_RIGHT_SUPER        347
#define KEY_MENU               348

#define KEY_LAST               KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *  @brief Modifier key flags.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  @ingroup input
 *  @{ */

 /*! @brief If this bit is set one or more Shift keys were held down.
  *
  *  If this bit is set one or more Shift keys were held down.
  */
#define KEY_MOD_SHIFT           0x0001
  /*! @brief If this bit is set one or more Control keys were held down.
   *
   *  If this bit is set one or more Control keys were held down.
   */
#define MOD_CONTROL         0x0002
   /*! @brief If this bit is set one or more Alt keys were held down.
    *
    *  If this bit is set one or more Alt keys were held down.
    */
#define KEY_MOD_ALT             0x0004
    /*! @brief If this bit is set one or more Super keys were held down.
     *
     *  If this bit is set one or more Super keys were held down.
     */
#define MOD_SUPER           0x0008
     /*! @brief If this bit is set the Caps Lock key is enabled.
      *
      *  If this bit is set the Caps Lock key is enabled and the @ref
      *  LOCK_KEY_MODS input mode is set.
      */
#define MOD_CAPS_LOCK       0x0010
      /*! @brief If this bit is set the Num Lock key is enabled.
       *
       *  If this bit is set the Num Lock key is enabled and the @ref
       *  LOCK_KEY_MODS input mode is set.
       */
#define MOD_NUM_LOCK        0x0020

#define NUM_KEYS 120

enum KeyCode : uint32_t
{
    Key_Invalid = 0,
    Key_Space = KEY_SPACE,
    Key_Apostrophe = KEY_APOSTROPHE,
    Key_Coma = KEY_COMMA,
    Key_Minus = KEY_MINUS,
    Key_Period = KEY_PERIOD,
    Key_Slash = KEY_SLASH,
    Key_Num0 = KEY_0,
    Key_Num1 = KEY_1,
    Key_Num2 = KEY_2,
    Key_Num3 = KEY_3,
    Key_Num4 = KEY_4,
    Key_Num5 = KEY_5,
    Key_Num6 = KEY_6,
    Key_Num7 = KEY_7,
    Key_Num8 = KEY_8,
    Key_Num9 = KEY_9,
    Key_Semicolon = KEY_SEMICOLON,
    Key_Equal = KEY_EQUAL,
    Key_A = KEY_A,
    Key_B = KEY_B,
    Key_C = KEY_C,
    Key_D = KEY_D,
    Key_E = KEY_E,
    Key_F = KEY_F,
    Key_G = KEY_G,
    Key_H = KEY_H,
    Key_I = KEY_I,
    Key_J = KEY_J,
    Key_K = KEY_K,
    Key_L = KEY_L,
    Key_M = KEY_M,
    Key_N = KEY_N,
    Key_O = KEY_O,
    Key_P = KEY_P,
    Key_Q = KEY_Q,
    Key_R = KEY_R,
    Key_S = KEY_S,
    Key_T = KEY_T,
    Key_U = KEY_U,
    Key_V = KEY_V,
    Key_W = KEY_W,
    Key_X = KEY_X,
    Key_Y = KEY_Y,
    Key_Z = KEY_Z,
    Key_LeftBracket = KEY_LEFT_BRACKET,
    Key_BackSlash = KEY_BACKSLASH,
    Key_RightBracket = KEY_RIGHT_BRACKET,
    Key_GraveAccent = KEY_GRAVE_ACCENT,
    Key_World1 = KEY_WORLD_1,
    Key_World2 = KEY_WORLD_2,
    Key_Escape = KEY_ESCAPE,
    Key_Enter = KEY_ENTER,
    Key_Tab = KEY_TAB,
    Key_BackSpace = KEY_BACKSPACE,
    Key_Insert = KEY_INSERT,
    Key_Delete = KEY_DELETE,
    Key_Right = KEY_RIGHT,
    Key_Left = KEY_LEFT,
    Key_Down = KEY_DOWN,
    Key_Up = KEY_UP,
    Key_PageUp = KEY_PAGE_UP,
    Key_PageDown = KEY_PAGE_DOWN,
    Key_Home = KEY_HOME,
    Key_End = KEY_END,
    Key_CapsLock = KEY_CAPS_LOCK,
    Key_ScrollLock = KEY_SCROLL_LOCK,
    Key_NumLock = KEY_NUM_LOCK,
    Key_PrintScreen = KEY_PRINT_SCREEN,
    Key_Pause = KEY_PAUSE,
    Key_F1 = KEY_F1,
    Key_F2 = KEY_F2,
    Key_F3 = KEY_F3,
    Key_F4 = KEY_F4,
    Key_F5 = KEY_F5,
    Key_F6 = KEY_F6,
    Key_F7 = KEY_F7,
    Key_F8 = KEY_F8,
    Key_F9 = KEY_F9,
    Key_F10 = KEY_F10,
    Key_F11 = KEY_F11,
    Key_F12 = KEY_F12,
    Key_F13 = KEY_F13,
    Key_F14 = KEY_F14,
    Key_F15 = KEY_F15,
    Key_F16 = KEY_F16,
    Key_F17 = KEY_F17,
    Key_F18 = KEY_F18,
    Key_F19 = KEY_F19,
    Key_F20 = KEY_F20,
    Key_F21 = KEY_F21,
    Key_F22 = KEY_F22,
    Key_F23 = KEY_F23,
    Key_F24 = KEY_F24,
    Key_F25 = KEY_F25,
    Key_KP0 = KEY_KP_0,
    Key_KP1 = KEY_KP_1,
    Key_KP2 = KEY_KP_2,
    Key_KP3 = KEY_KP_3,
    Key_KP4 = KEY_KP_4,
    Key_KP5 = KEY_KP_5,
    Key_KP6 = KEY_KP_6,
    Key_KP7 = KEY_KP_7,
    Key_KP8 = KEY_KP_8,
    Key_KP9 = KEY_KP_9,
    Key_KPDecimal = KEY_KP_DECIMAL,
    Key_KPDivide = KEY_KP_DIVIDE,
    Key_KPMultiply = KEY_KP_MULTIPLY,
    Key_KPSubstract = KEY_KP_SUBTRACT,
    Key_KPAdd = KEY_KP_ADD,
    Key_KPEnter = KEY_KP_ENTER,
    Key_KPEqual = KEY_KP_EQUAL,
    Key_LeftShift = KEY_LEFT_SHIFT,
    Key_LeftControl = KEY_LEFT_CONTROL,
    Key_LeftAlt = KEY_LEFT_ALT,
    Key_LeftSuper = KEY_LEFT_SUPER,
    Key_RightShift = KEY_RIGHT_SHIFT,
    Key_RightControl = KEY_RIGHT_CONTROL,
    Key_RightAlt = KEY_RIGHT_ALT,
    Key_RightSuper = KEY_RIGHT_SUPER,
    Key_Menu = KEY_MENU,
};

       /*! @} */

       /*! @defgroup buttons Mouse buttons
        *  @brief Mouse button IDs.
        *
        *  See [mouse button input](@ref input_mouse_button) for how these are used.
        *
        *  @ingroup input
        *  @{ */
#define MOUSE_BUTTON_1         0
#define MOUSE_BUTTON_2         1
#define MOUSE_BUTTON_3         2
#define MOUSE_BUTTON_4         3
#define MOUSE_BUTTON_5         4
#define MOUSE_BUTTON_6         5
#define MOUSE_BUTTON_7         6
#define MOUSE_BUTTON_8         7
#define MOUSE_BUTTON_LAST      MOUSE_BUTTON_8
#define MOUSE_BUTTON_LEFT      MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT     MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE    MOUSE_BUTTON_3
        /*! @} */

        /*! @defgroup joysticks Joysticks
         *  @brief Joystick IDs.
         *
         *  See [joystick input](@ref joystick) for how these are used.
         *
         *  @ingroup input
         *  @{ */
#define JOYSTICK_1             0
#define JOYSTICK_2             1
#define JOYSTICK_3             2
#define JOYSTICK_4             3
#define JOYSTICK_5             4
#define JOYSTICK_6             5
#define JOYSTICK_7             6
#define JOYSTICK_8             7
#define JOYSTICK_9             8
#define JOYSTICK_10            9
#define JOYSTICK_11            10
#define JOYSTICK_12            11
#define JOYSTICK_13            12
#define JOYSTICK_14            13
#define JOYSTICK_15            14
#define JOYSTICK_16            15
#define JOYSTICK_LAST          JOYSTICK_16
         /*! @} */

         /*! @defgroup gamepad_buttons Gamepad buttons
          *  @brief Gamepad buttons.
          *
          *  See @ref gamepad for how these are used.
          *
          *  @ingroup input
          *  @{ */
#define GAMEPAD_BUTTON_A               0
#define GAMEPAD_BUTTON_B               1
#define GAMEPAD_BUTTON_X               2
#define GAMEPAD_BUTTON_Y               3
#define GAMEPAD_BUTTON_LEFT_BUMPER     4
#define GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define GAMEPAD_BUTTON_BACK            6
#define GAMEPAD_BUTTON_START           7
#define GAMEPAD_BUTTON_GUIDE           8
#define GAMEPAD_BUTTON_LEFT_THUMB      9
#define GAMEPAD_BUTTON_RIGHT_THUMB     10
#define GAMEPAD_BUTTON_DPAD_UP         11
#define GAMEPAD_BUTTON_DPAD_RIGHT      12
#define GAMEPAD_BUTTON_DPAD_DOWN       13
#define GAMEPAD_BUTTON_DPAD_LEFT       14
#define GAMEPAD_BUTTON_LAST            GAMEPAD_BUTTON_DPAD_LEFT

#define GAMEPAD_BUTTON_CROSS       GAMEPAD_BUTTON_A
#define GAMEPAD_BUTTON_CIRCLE      GAMEPAD_BUTTON_B
#define GAMEPAD_BUTTON_SQUARE      GAMEPAD_BUTTON_X
#define GAMEPAD_BUTTON_TRIANGLE    GAMEPAD_BUTTON_Y
          /*! @} */

          /*! @defgroup gamepad_axes Gamepad axes
           *  @brief Gamepad axes.
           *
           *  See @ref gamepad for how these are used.
           *
           *  @ingroup input
           *  @{ */
#define GAMEPAD_AXIS_LEFT_X        0
#define GAMEPAD_AXIS_LEFT_Y        1
#define GAMEPAD_AXIS_RIGHT_X       2
#define GAMEPAD_AXIS_RIGHT_Y       3
#define GAMEPAD_AXIS_LEFT_TRIGGER  4
#define GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define GAMEPAD_AXIS_LAST          GAMEPAD_AXIS_RIGHT_TRIGGER
           /*! @} */

struct WiimoteKeyNames
{
    inline static const char* Wiimote_DPad_Up = "Wiimote_DPad_Up";
    inline static const char* Wiimote_DPad_Down = "Wiimote_DPad_Down";
    inline static const char* Wiimote_DPad_Right = "Wiimote_DPad_Right";
    inline static const char* Wiimote_DPad_Left = "Wiimote_DPad_Left";
    inline static const char* Wiimote_Plus = "Wiimote_Plus";
    inline static const char* Wiimote_Home = "Wiimote_Home";
    inline static const char* Wiimote_Minus = "Wiimote_Minus";
    inline static const char* Wiimote_Button_A = "Wiimote_Button_A";
    inline static const char* Wiimote_Button_B = "Wiimote_Button_B";
    inline static const char* Wiimote_Button_1 = "Wiimote_Button_1";
    inline static const char* Wiimote_Button_2 = "Wiimote_Button_2";
    inline static const char* Wiimote_PointerVisible = "Wiimote_PointerVisible";
    inline static const char* Wiimote_PointerDistance = "Wiimote_PointerDistance";
    inline static const char* Wiimote_PointerAngle = "Wiimote_PointerAngle";
    inline static const char* Wiimote_PointerX = "Wiimote_PointerX";
    inline static const char* Wiimote_PointerY = "Wiimote_PointerY";
    inline static const char* Wiimote_Pointer2D = "Wiimote_Pointer2D";
    inline static const char* Wiimote_BatteryLevel = "Wiimote_BatteryLevel";
    inline static const char* Nunchuck_Button_C = "Nunchuck_Button_C";
    inline static const char* Nunchuck_Button_Z = "Nunchuck_Button_Z";
    inline static const char* Nunchuck_StickX = "Nunchuck_StickX";
    inline static const char* Nunchuck_StickY = "Nunchuck_StickY";
    inline static const char* Nunchuck_Stick2D = "Nunchuck_Stick2D";
    inline static const char* Wiimote_TiltX = "Wiimote_TiltX";
    inline static const char* Wiimote_TiltY = "Wiimote_TiltY";
    inline static const char* Wiimote_TiltZ = "Wiimote_TiltZ";
    inline static const char* Wiimote_Tilt3D = "Wiimote_Tilt3D";
    inline static const char* Wiimote_RotationRateX = "Wiimote_RotationRateX";
    inline static const char* Wiimote_RotationRateY = "Wiimote_RotationRateY";
    inline static const char* Wiimote_RotationRateZ = "Wiimote_RotationRateZ";
    inline static const char* Wiimote_RotationRate3D = "Wiimote_RotationRate3D";
    inline static const char* Wiimote_GravityX = "Wiimote_GravityX";
    inline static const char* Wiimote_GravityY = "Wiimote_GravityY";
    inline static const char* Wiimote_GravityZ = "Wiimote_GravityZ";
    inline static const char* Wiimote_Gravity3D = "Wiimote_Gravity3D";
    inline static const char* Wiimote_AccelerationX = "Wiimote_AccelerationX";
    inline static const char* Wiimote_AccelerationY = "Wiimote_AccelerationY";
    inline static const char* Wiimote_AccelerationZ = "Wiimote_AccelerationZ";
    inline static const char* Wiimote_Acceleration3D = "Wiimote_Acceleration3D";
};

struct KeyboardKeyNames
{
    inline static const char* Key_Space = "Key_Space";
    inline static const char* Key_Apostrophe = "Key_Apostrophe";
    inline static const char* Key_Coma = "Key_Coma";
    inline static const char* Key_Minus = "Key_Minus";
    inline static const char* Key_Period = "Key_Period";
    inline static const char* Key_Slash = "Key_Slash";
    inline static const char* Key_Num0 = "Key_Num0";
    inline static const char* Key_Num1 = "Key_Num1";
    inline static const char* Key_Num2 = "Key_Num2";
    inline static const char* Key_Num3 = "Key_Num3";
    inline static const char* Key_Num4 = "Key_Num4";
    inline static const char* Key_Num5 = "Key_Num5";
    inline static const char* Key_Num6 = "Key_Num6";
    inline static const char* Key_Num7 = "Key_Num7";
    inline static const char* Key_Num8 = "Key_Num8";
    inline static const char* Key_Num9 = "Key_Num9";
    inline static const char* Key_Semicolon = "Key_Semicolon";
    inline static const char* Key_Equal = "Key_Equal";
    inline static const char* Key_A = "Key_A";
    inline static const char* Key_B = "Key_B";
    inline static const char* Key_C = "Key_C";
    inline static const char* Key_D = "Key_D";
    inline static const char* Key_E = "Key_E";
    inline static const char* Key_F = "Key_F";
    inline static const char* Key_G = "Key_G";
    inline static const char* Key_H = "Key_H";
    inline static const char* Key_I = "Key_I";
    inline static const char* Key_J = "Key_J";
    inline static const char* Key_K = "Key_K";
    inline static const char* Key_L = "Key_L";
    inline static const char* Key_M = "Key_M";
    inline static const char* Key_N = "Key_N";
    inline static const char* Key_O = "Key_O";
    inline static const char* Key_P = "Key_P";
    inline static const char* Key_Q = "Key_Q";
    inline static const char* Key_R = "Key_R";
    inline static const char* Key_S = "Key_S";
    inline static const char* Key_T = "Key_T";
    inline static const char* Key_U = "Key_U";
    inline static const char* Key_V = "Key_V";
    inline static const char* Key_W = "Key_W";
    inline static const char* Key_X = "Key_X";
    inline static const char* Key_Y = "Key_Y";
    inline static const char* Key_Z = "Key_Z";
    inline static const char* Key_LeftBracket = "Key_LeftBracket";
    inline static const char* Key_BackSlash = "Key_BackSlash";
    inline static const char* Key_RightBracket = "Key_RightBracket";
    inline static const char* Key_GraveAccent = "Key_GraveAccent";
    inline static const char* Key_World1 = "Key_World1";
    inline static const char* Key_World2 = "Key_World2";
    inline static const char* Key_Escape = "Key_Escape";
    inline static const char* Key_Enter = "Key_Enter";
    inline static const char* Key_Tab = "Key_Tab";
    inline static const char* Key_BackSpace = "Key_BackSpace";
    inline static const char* Key_Insert = "Key_Insert";
    inline static const char* Key_Delete = "Key_Delete";
    inline static const char* Key_Right = "Key_Right";
    inline static const char* Key_Left = "Key_Left";
    inline static const char* Key_Down = "Key_Down";
    inline static const char* Key_Up = "Key_Up";
    inline static const char* Key_PageUp = "Key_PageUp";
    inline static const char* Key_PageDown = "Key_PageDown";
    inline static const char* Key_Home = "Key_Home";
    inline static const char* Key_End = "Key_End";
    inline static const char* Key_CapsLock = "Key_CapsLock";
    inline static const char* Key_ScrollLock = "Key_ScrollLock";
    inline static const char* Key_NumLock = "Key_NumLock";
    inline static const char* Key_PrintScreen = "Key_PrintScreen";
    inline static const char* Key_Pause = "Key_Pause";
    inline static const char* Key_F1 = "Key_F1";
    inline static const char* Key_F2 = "Key_F2";
    inline static const char* Key_F3 = "Key_F3";
    inline static const char* Key_F4 = "Key_F4";
    inline static const char* Key_F5 = "Key_F5";
    inline static const char* Key_F6 = "Key_F6";
    inline static const char* Key_F7 = "Key_F7";
    inline static const char* Key_F8 = "Key_F8";
    inline static const char* Key_F9 = "Key_F9";
    inline static const char* Key_F10 = "Key_F10";
    inline static const char* Key_F11 = "Key_F11";
    inline static const char* Key_F12 = "Key_F12";
    inline static const char* Key_F13 = "Key_F13";
    inline static const char* Key_F14 = "Key_F14";
    inline static const char* Key_F15 = "Key_F15";
    inline static const char* Key_F16 = "Key_F16";
    inline static const char* Key_F17 = "Key_F17";
    inline static const char* Key_F18 = "Key_F18";
    inline static const char* Key_F19 = "Key_F19";
    inline static const char* Key_F20 = "Key_F20";
    inline static const char* Key_F21 = "Key_F21";
    inline static const char* Key_F22 = "Key_F22";
    inline static const char* Key_F23 = "Key_F23";
    inline static const char* Key_F24 = "Key_F24";
    inline static const char* Key_F25 = "Key_F25";
    inline static const char* Key_KP0 = "Key_KP0";
    inline static const char* Key_KP1 = "Key_KP1";
    inline static const char* Key_KP2 = "Key_KP2";
    inline static const char* Key_KP3 = "Key_KP3";
    inline static const char* Key_KP4 = "Key_KP4";
    inline static const char* Key_KP5 = "Key_KP5";
    inline static const char* Key_KP6 = "Key_KP6";
    inline static const char* Key_KP7 = "Key_KP7";
    inline static const char* Key_KP8 = "Key_KP8";
    inline static const char* Key_KP9 = "Key_KP9";
    inline static const char* Key_KPDecimal = "Key_KPDecimal";
    inline static const char* Key_KPDivide = "Key_KPDivide";
    inline static const char* Key_KPMultiply = "Key_KPMultiply";
    inline static const char* Key_KPSubstract = "Key_KPSubstract";
    inline static const char* Key_KPAdd = "Key_KPAdd";
    inline static const char* Key_KPEnter = "Key_KPEnter";
    inline static const char* Key_KPEqual = "Key_KPEqual";
    inline static const char* Key_LeftShift = "Key_LeftShift";
    inline static const char* Key_LeftControl = "Key_LeftControl";
    inline static const char* Key_LeftAlt = "Key_LeftAlt";
    inline static const char* Key_LeftSuper = "Key_LeftSuper";
    inline static const char* Key_RightShift = "Key_RightShift";
    inline static const char* Key_RightControl = "Key_RightControl";
    inline static const char* Key_RightAlt = "Key_RightAlt";
    inline static const char* Key_RightSuper = "Key_RightSuper";
    inline static const char* Key_Menu = "Key_Menu";
};