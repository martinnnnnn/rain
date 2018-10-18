#include "core/input.h"

#include <iostream>

using namespace rain;

void Input::initialize_center_pos(int _x, int _y)
{
    centerPosX = _x;
    centerPosY = _y;
}

void Input::update()
{
    m_inputEvents.clear();
    reset_mouse_offset();
}

void Input::add_input_event(InputEvent _event)
{
    m_inputEvents.push_back(_event);
    if (_event.type == InputEvent::Type::MouseMotion)
    {
        if (_event.X != centerPosX || _event.Y != centerPosY)
        {
            offsetX = _event.X;
            offsetY = _event.Y;
        }
        else
        {
            std::cout << "hello";
        }
    }
}

void Input::reset_mouse_offset()
{
    offsetX = 0;
    offsetY = 0;
}


bool Input::get_input(KeyCode::Key _key)
{
    bool retval = false;
    for (auto input_event : m_inputEvents)
    {
        if (input_event.type == InputEvent::Type::Keyboard && input_event.state == InputEvent::State::Pressed && input_event.key == _key)
        {
            retval = true;
            break;
        }
    }
    return retval;
}