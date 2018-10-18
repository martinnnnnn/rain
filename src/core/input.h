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
            , centerPosX(0)
            , centerPosY(0)
        {
        }

        void initialize_center_pos(int _x, int _y);
        void update();
        void add_input_event(InputEvent _event);
        bool get_input(KeyCode::Key _key);
        void reset_mouse_offset();

        int offsetX;
        int offsetY;
        int centerPosX;
        int centerPosY;

    private:
        std::vector<InputEvent> m_inputEvents;
    };

}
#define GETINPUT rain::Input::Get()
