#include "event.h"



InputEvent InputEvent::create_keyboard_event(KeyCode::Key _key, u32 _char, InputEvent::State _state, bool _ctrl, bool _shift, bool _alt)
{
    InputEvent inputEvent = {};
    inputEvent.type = InputEvent::Type::Keyboard;
    inputEvent.key = _key;
    inputEvent.character = _char;
    inputEvent.state = _state;
    inputEvent.Control = _ctrl;
    inputEvent.Shift = _shift;
    inputEvent.Alt = _alt;
    return inputEvent;
}

InputEvent InputEvent::create_mouse_motion_event(int _x, int _y, bool _leftButton, bool _rightButton, bool _middleButton, bool _ctrl, bool _shift, bool _alt)
{
    InputEvent inputEvent = {};
    inputEvent.type = InputEvent::Type::MouseMotion;
    inputEvent.X = _x;
    inputEvent.Y = _y;
    inputEvent.LeftButton = _leftButton;
    inputEvent.RightButton = _rightButton;
    inputEvent.MiddleButton = _middleButton;
    inputEvent.Control = _ctrl;
    inputEvent.Shift = _shift;
    inputEvent.Alt = _alt;
    return inputEvent;
}

InputEvent InputEvent::create_mouse_button_event(InputEvent::MouseButtonType _buttonType, InputEvent::State _state, int _x, int _y, bool _leftButton, bool _rightButton, bool _middleButton, bool _ctrl, bool _shift, bool _alt)
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
    inputEvent.Control = _ctrl;
    inputEvent.Shift = _shift;
    inputEvent.Alt = _alt;
    return inputEvent;
}