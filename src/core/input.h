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

        void update();
        void push_back(InputEvent _event);
        bool get_input(KeyCode::Key _key);

        
        int offsetX;
        int offsetY;
        int lastPosX;
        int lastPosY;

    private:
        std::vector<InputEvent> m_inputEvents;
    };

}
#define GETINPUT rain::Input::Get()
