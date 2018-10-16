#pragma once

#include <vector>
#include <algorithm>

#include "singleton.h"
#include "event.h"
#include "win32/win32_keycodes.h"

namespace rain
{
    class Input : public Singleton<Input>
    {
    public:
        Input()
            : offsetX(0)
            , offsetY(0)
            , lastPosX(-1)
            , lastPosY(-1)
        {
        }

        void update()
        {
            m_inputEvents.clear();
        }

        void push_back(InputEvent _event)
        {
            m_inputEvents.push_back(_event);
            if (_event.type == InputEvent::Type::MouseMotion)
            {
                if (lastPosX == -1 || lastPosY == -1)
                {
                    lastPosX = (float)_event.X;
                    lastPosY = (float)_event.Y;
                }

                offsetX = (float)_event.X - lastPosX;
                offsetY = lastPosY - (float)_event.Y;
                lastPosX = (float)_event.X;
                lastPosY = (float)_event.Y;
            }
        }

        bool get_input(KeyCode::Key _key)
        {
            for (auto input_event : m_inputEvents)
            {
                return (input_event.type == InputEvent::Type::Keyboard && input_event.key == _key);
            }
        }

        
        float offsetX;
        float offsetY;
        float lastPosX;
        float lastPosY;

    private:
        std::vector<InputEvent> m_inputEvents;
    };
}