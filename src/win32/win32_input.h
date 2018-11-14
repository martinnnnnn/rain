#pragma once

#include <vector>
#include <algorithm>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <glm/glm.hpp>

#include "core/singleton.h"
#include "core/event.h"
#include "win32/win32_keycodes.h"
#include "win32/win32_application.h"

namespace rain
{
    class Input
    {
    public:
        Input();

        int init();
        void shutdown();

        void update();

        void set_cursor_pos(u32 _x, u32 _y);
        void update_absolute_mouse_pos(u32 _x, u32 _y);

        bool is_key_pressed(u32 _code);

        i32 x_offset;
        i32 y_offset;
        u32 x_screen_pos;
        u32 y_screen_pos;
        bool mouse_lock;
    private:
        std::vector<InputEvent> m_inputEvents;
        LPDIRECTINPUT8  m_diObject;
        LPDIRECTINPUTDEVICE8 m_keyboard;
        LPDIRECTINPUTDEVICE8 m_mouse;
        BYTE m_keys[256];
        u32 x_center;
        u32 y_center;
    };

}

#define RAIN_INPUT rain::Application::get().input
