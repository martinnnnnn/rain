#pragma once

#include <vector>
#include <array>
#include <algorithm>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "engine/core/event.h"
#include "engine/win32/win32_keycodes.h"
#include "engine/win32/win32_application.h"

namespace rain::engine
{
    class Input
    {
    public:
        Input();

        int init();
        void shutdown();
        void update();

        void set_cursor_pos(const i32 _x, const i32 _y);

        bool is_key_pressed(u32 _code) const;
        bool is_key_released(u32 _code)  const;

        i32 x_frame_offset;
        i32 y_frame_offset;
        i32 x_from_topleft;
        i32 y_from_topleft;
        bool mouse_lock;
        bool mouseclick_left;
        bool mouseclick_right;
        i8 mouse_wheel;
    private:
        static constexpr u32 KEYS_COUNT = 256;

        std::vector<InputEvent> m_inputEvents;
        LPDIRECTINPUT8  m_diObject;
        LPDIRECTINPUTDEVICE8 m_keyboard;
        LPDIRECTINPUTDEVICE8 m_mouse;
        std::array<BYTE, KEYS_COUNT> m_keysA;
        std::array<BYTE, KEYS_COUNT> m_keysB;
        std::array<BYTE, KEYS_COUNT>* m_keysCurrent;
        u32 x_center;
        u32 y_center;
    };

}