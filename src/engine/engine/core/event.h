#pragma once

#include "engine/win32/win32_keycodes.h"
#include "core/core.h"

struct InputEvent
{
    enum class Type
    {
        Keyboard = 0,
        MouseMotion = 1,
        MouseButton = 3,
        MouseWheel = 4
    };

    enum class State
    {
        Released = 0,
        Pressed = 1
    };

    enum class MouseButtonType
    {
        None = 0,
        Left = 1,
        Right = 2,
        Middel = 3
    };

    Type type;

    int X;                      // The X-position of the cursor relative to the upper-left corner of the client area.
    int Y;                      // The Y-position of the cursor relative to the upper-left corner of the client area.
    bool LeftButton;
    bool MiddleButton;
    bool RightButton;
    bool Control;
    bool Shift;
    bool Alt;

                                // specific to "Keyboard" event
    KeyCode::Key    key;        // The Key Code that was pressed or released.
    u32             character;  // The 32-bit character code that was pressed. This value will be 0 if it is a non-printable character.
    State           state;      // Was the key pressed or released?

                                // specific to "Button" event
    MouseButtonType Button;

                                // specific to "Wheel" event
    float WheelDelta;           // How much the mouse wheel has moved. A positive value indicates that the wheel was moved to the right. A negative value indicates the wheel was moved to the left.

    static InputEvent create_keyboard_event(KeyCode::Key _key, u32 _char, State _state, bool _ctrl = false, bool _shift = false, bool _alt = false);
    static InputEvent create_mouse_motion_event(int _x, int _y, bool _leftButton = false, bool _rightButton = false, bool _middleButton = false, bool _ctrl = false, bool _shift = false, bool _alt = false);
    static InputEvent create_mouse_button_event(MouseButtonType _buttonType, State _state, int _x, int _y, bool _leftButton = false, bool _rightButton = false, bool _middleButton = false, bool _ctrl = false, bool _shift = false, bool _alt = false);

};

class ResizeEvent
{
public:
    ResizeEvent(int width, int height)
        : Width(width)
        , Height(height)
    {}

    // The new width of the window
    int Width;
    // The new height of the window.
    int Height;

};

struct RIEvent
{

};