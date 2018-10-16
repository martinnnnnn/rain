#pragma once
#include "win32/win32_keycodes.h"


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

    enum class ButtonType
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
    unsigned int    character;  // The 32-bit character code that was pressed. This value will be 0 if it is a non-printable character.
    State        state;         // Was the key pressed or released?

                                // specific to "Button" event
    ButtonType Button;

                                // specific to "Wheel" event
    float WheelDelta;           // How much the mouse wheel has moved. A positive value indicates that the wheel was moved to the right. A negative value indicates the wheel was moved to the left.

    static InputEvent create_keyboard_event(KeyCode::Key _key, State _state, bool _shift, bool _ctrl, bool _alt);
    static InputEvent create_mouse_move_event(int _x, int _y, bool _leftButton, bool _rightButton, bool _middleButton, bool _shift, bool _ctrl, bool _alt);
    static InputEvent create_mouse_button_event(ButtonType _buttonType, State _state, int _x, int _y, bool _leftButton, bool _rightButton, bool _middleButton, bool _shift, bool _ctrl, bool _alt);

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


InputEvent InputEvent::create_keyboard_event(KeyCode::Key _key, InputEvent::State _state, bool _shift, bool _ctrl, bool _alt)
{
    InputEvent inputEvent = {};
    inputEvent.type = InputEvent::Type::Keyboard;
    inputEvent.key = _key;
    inputEvent.state = _state;
    inputEvent.Shift = _shift;
    inputEvent.Control = _ctrl;
    inputEvent.Alt = _alt;
    return inputEvent;
}

InputEvent InputEvent::create_mouse_move_event(int _x, int _y, bool _leftButton, bool _rightButton, bool _middleButton, bool _shift, bool _ctrl, bool _alt)
{
    InputEvent inputEvent = {};
    inputEvent.type = InputEvent::Type::MouseMotion;
    inputEvent.X = _x;
    inputEvent.Y = _y;
    inputEvent.LeftButton = _leftButton;
    inputEvent.RightButton = _rightButton;
    inputEvent.MiddleButton = _middleButton;
    inputEvent.Shift = _shift;
    inputEvent.Control = _ctrl;
    inputEvent.Alt = _alt;
    return inputEvent;
}

InputEvent InputEvent::create_mouse_button_event(InputEvent::ButtonType _buttonType, InputEvent::State _state, int _x, int _y, bool _leftButton, bool _rightButton, bool _middleButton, bool _shift, bool _ctrl, bool _alt)
{
    InputEvent inputEvent = {};
    inputEvent.type = InputEvent::Type::MouseMotion;
    inputEvent.Button = _buttonType;
    inputEvent.state = _state;
    inputEvent.X = _x;
    inputEvent.Y = _y;
    inputEvent.LeftButton = _leftButton;
    inputEvent.RightButton = _rightButton;
    inputEvent.MiddleButton = _middleButton;
    inputEvent.Shift = _shift;
    inputEvent.Control = _ctrl;
    inputEvent.Alt = _alt;
    return inputEvent;
}


#include <Windows.h>

InputEvent::ButtonType DecodeMouseButton(UINT messageID);