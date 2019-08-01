#include "win32_input.h"

#include <iostream>

#include "engine/core/context.h"
#include "engine/win32/win32_window.h"
#include "engine/win32/win32_application.h"

using namespace rain::engine;


Input::Input()
    : x_frame_offset(0)
    , y_frame_offset(0)
    , x_from_topleft(0)
    , y_from_topleft(0)
    , mouse_lock(0)
    , mouseclick_left(mouse_state::released)
    , mouseclick_left_time(0)
    , mouseclick_right(mouse_state::released)
    , mouseclick_right_time(0)
    , mouse_wheel(0)
    , m_inputEvents(0)
    , m_diObject(0)
    , m_keyboard(0)
    , m_mouse(0)
    , m_keysCurrent(&m_keysA)
    , x_center(0)
    , y_center(0)

{

    ZeroMemory(m_keysA.data(), m_keysA.size());
    ZeroMemory(m_keysB.data(), m_keysB.size());
}

int Input::init()
{
    init_alphanum_codes();

    // INITIALIZING INPUT
    HRESULT hr = ::DirectInput8Create(::GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_diObject, NULL);
    if (FAILED(hr))
        return hr;

    hr = m_diObject->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
    if (FAILED(hr))
        return hr;

    hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr))
        return hr;

    hr = m_keyboard->SetCooperativeLevel(RAIN_WINDOW->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr))
        return hr;

    hr = m_diObject->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
    if (FAILED(hr))
        return hr;

    hr = m_mouse->SetDataFormat(&c_dfDIMouse2);
    if (FAILED(hr))
        return hr;

    hr = m_mouse->SetCooperativeLevel(RAIN_WINDOW->hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    if (FAILED(hr))
        return hr;

    //if (!bImmediate)
    {
        DIPROPDWORD dipdw;
        dipdw.diph.dwSize = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj = 0;
        dipdw.diph.dwHow = DIPH_DEVICE;
        dipdw.dwData = 16; // Arbitrary buffer size

        if (FAILED(hr = m_mouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
            return hr;
    }

    m_keyboard->Acquire();
    m_mouse->Acquire();



    return 0;
}

void Input::shutdown()
{
    if (m_keyboard)
    {
        m_keyboard->Unacquire();
        m_keyboard->Release();
    }

    if (m_mouse)
    {
        m_mouse->Unacquire();
        m_mouse->Release();
    }

    if (m_diObject)
    {
        m_diObject->Release();
    }
}


void Input::update()
{
    m_keyboard->Acquire();

    if (m_keysCurrent == &m_keysA)
    {
        m_keysCurrent = &m_keysB;
    }
    else
    {
        m_keysCurrent = &m_keysA;
    }

    ZeroMemory(m_keysCurrent->data(), m_keysCurrent->size());
    HRESULT hr = m_keyboard->GetDeviceState(m_keysCurrent->size(), m_keysCurrent->data());

    if (FAILED(hr))
    {
        hr = m_keyboard->Acquire();
        while (hr == DIERR_INPUTLOST)
        {
            hr = m_keyboard->Acquire();
        }
        if (FAILED(hr))
            return;

        m_keyboard->GetDeviceState(m_keysCurrent->size(), m_keysCurrent->data());
    }

    // MOUSE
    if (is_key_released(DIK_C) && is_key_pressed(DIK_LCONTROL))
    {
        mouse_lock = !mouse_lock;
    }

    if (is_key_released(DIK_V) && is_key_pressed(DIK_LCONTROL))
    {
        RAIN_WINDOW->toggle_fullscreen();
    }

    DIMOUSESTATE2 dims2{};

    hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &dims2);
    if (FAILED(hr))
    {
        hr = m_mouse->Acquire();
        while (hr == DIERR_INPUTLOST)
        {
            hr = m_mouse->Acquire();
        }

        if (FAILED(hr))
            return;
    }

    x_frame_offset = dims2.lX;
    y_frame_offset = -dims2.lY;
    mouse_wheel = i8(dims2.lZ);
    
    // update mouse click up and down
    update_mouse_click(dims2.rgbButtons[0], mouseclick_left, mouseclick_left_time);
    update_mouse_click(dims2.rgbButtons[1], mouseclick_right, mouseclick_right_time);

    if (mouse_lock)
    {
		auto pos = RAIN_WINDOW->get_center_pos_absolute();
		x_center = u32(pos.x);
		y_center = u32(pos.y);
        set_cursor_pos(x_center, y_center);
    }

	Window::rect rect = RAIN_WINDOW->get_rect();
	POINT monitor_position;
	::GetCursorPos(&monitor_position);

	x_from_topleft = monitor_position.x - rect.left;
	y_from_topleft = monitor_position.y - rect.top;

    char buffer[512];
    core::string::print_to_buffer(buffer, 512, "(%d, %d)", x_from_topleft, y_from_topleft);
    RAIN_RENDERER->draw_text_2d(buffer, 500, 50, 0.5f, glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
}

void Input::update_mouse_click(const bool input_state, mouse_state& state,  f64& time)
{
    time += RAIN_CLOCK->get_delta_milliseconds();
    switch (state)
    {
    case mouse_state::pressed:
    {
        if (!input_state)
        {
            state = mouse_state::released;
            time = 0.0;
        }
        break;
    }
    case mouse_state::released:
    {
        if (input_state)
        {
            state = mouse_state::pressed;
            time = 0.0;
        }
        break;
    }
    }
}


bool Input::is_key_pressed(u32 _code) const
{
    return ((*m_keysCurrent)[_code] & 0x80);
    //return (m_keysCurrent[_code] & 0x80);
}

bool Input::is_key_released(u32 _code) const
{
    if (!is_key_pressed(_code))
    {
        if (m_keysCurrent == &m_keysA)
        {
            return (m_keysB[_code] & 0x80);
        }
        else
        {
            return (m_keysA[_code] & 0x80);
        }
    }
    return false;
}

void Input::set_cursor_pos(const i32 _x, const i32 _y)
{
    ::SetCursorPos(_x, _y);
}

void Input::init_alphanum_codes()
{
    alphanum_codes[0] = DIK_A;
    alphanum_codes[1] = DIK_B;
    alphanum_codes[2] = DIK_C;
    alphanum_codes[3] = DIK_D;
    alphanum_codes[4] = DIK_E;
    alphanum_codes[5] = DIK_F;
    alphanum_codes[6] = DIK_G;
    alphanum_codes[7] = DIK_H;
    alphanum_codes[8] = DIK_I;
    alphanum_codes[9] = DIK_J;
    alphanum_codes[10] = DIK_K;
    alphanum_codes[11] = DIK_L;
    alphanum_codes[12] = DIK_M;
    alphanum_codes[13] = DIK_N;
    alphanum_codes[14] = DIK_O;
    alphanum_codes[15] = DIK_P;
    alphanum_codes[16] = DIK_Q;
    alphanum_codes[17] = DIK_R;
    alphanum_codes[18] = DIK_S;
    alphanum_codes[19] = DIK_T;
    alphanum_codes[20] = DIK_U;
    alphanum_codes[21] = DIK_V;
    alphanum_codes[22] = DIK_W;
    alphanum_codes[23] = DIK_X;
    alphanum_codes[24] = DIK_Y;
    alphanum_codes[25] = DIK_Z;
    alphanum_codes[26] = DIK_1;
    alphanum_codes[27] = DIK_2;
    alphanum_codes[28] = DIK_3;
    alphanum_codes[29] = DIK_4;
    alphanum_codes[30] = DIK_5;
    alphanum_codes[31] = DIK_6;
    alphanum_codes[32] = DIK_7;
    alphanum_codes[33] = DIK_8;
    alphanum_codes[34] = DIK_9;
    alphanum_codes[35] = DIK_0;
    alphanum_codes[36] = DIK_SPACE;
}

char Input::get_char(u32 key_code)
{
    switch (key_code)
    {
    case DIK_A:
        return 'a';
    case DIK_B:
        return 'b';
    case DIK_C:
        return 'c';
    case DIK_D:
        return 'd';
    case DIK_E:
        return 'e';
    case DIK_F:
        return 'f';
    case DIK_G:
        return 'g';
    case DIK_H:
        return 'h';
    case DIK_I:
        return 'i';
    case DIK_J:
        return 'j';
    case DIK_K:
        return 'k';
    case DIK_L:
        return 'l';
    case DIK_M:
        return 'm';
    case DIK_N:
        return 'n';
    case DIK_O:
        return 'o';
    case DIK_P:
        return 'p';
    case DIK_Q:
        return 'q';
    case DIK_R:
        return 'r';
    case DIK_S:
        return 's';
    case DIK_T:
        return 't';
    case DIK_U:
        return 'u';
    case DIK_V:
        return 'v';
    case DIK_W:
        return 'w';
    case DIK_X:
        return 'x';
    case DIK_Y:
        return 'y';
    case DIK_Z:
        return 'z';
    case DIK_1:
        return '1';
    case DIK_2:
        return '2';
    case DIK_3:
        return '3';
    case DIK_4:
        return '4';
    case DIK_5:
        return '5';
    case DIK_6:
        return '6';
    case DIK_7:
        return '7';
    case DIK_8:
        return '8';
    case DIK_9:
        return '9';
    case DIK_0:
        return '0';
    case DIK_SPACE:
        return ' ';
    default:
        return ' ';
    }
}