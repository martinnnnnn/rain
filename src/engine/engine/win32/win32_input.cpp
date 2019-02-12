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
    , mouseclick_left(0)
    , mouseclick_right(0)
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
    mouseclick_left = dims2.rgbButtons[0];
    mouseclick_right = dims2.rgbButtons[1];
    
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
    RAIN_RENDERER->draw_text_2d(buffer, 25, 50, 0.5f, math::vec3{ 0.0f, 1.0f, 0.0f });
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