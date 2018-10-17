#include "core/input.h"

using namespace rain;

void Input::update()
{
    m_inputEvents.clear();
}

void Input::push_back(InputEvent _event)
{
    m_inputEvents.push_back(_event);
    if (_event.type == InputEvent::Type::MouseMotion)
    {
        if (lastPosX == -1.0f || lastPosY == -1.0f)
        {
            lastPosX = _event.X;
            lastPosY = _event.Y;
        }

        offsetX = lastPosX - _event.X;
        offsetY = lastPosY - _event.Y;
        lastPosX = _event.X;
        lastPosY = _event.Y;
    }
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