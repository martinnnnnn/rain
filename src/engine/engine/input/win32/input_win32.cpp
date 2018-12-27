#include "input_win32.h"


namespace rain::engine
{
	//InputWin32::InputWin32()
	//	: window_handle(0)
	//{

	//}

	//int InputWin32::init()
	//{
	//	// INITIALIZING INPUT
	//	HRESULT hr = ::DirectInput8Create(::GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_diObject, NULL);
	//	if (FAILED(hr))
	//		return hr;

	//	hr = m_diObject->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	//	if (FAILED(hr))
	//		return hr;

	//	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	//	if (FAILED(hr))
	//		return hr;

	//	hr = m_keyboard->SetCooperativeLevel(window_handle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	//	if (FAILED(hr))
	//		return hr;

	//	hr = m_diObject->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	//	if (FAILED(hr))
	//		return hr;

	//	hr = m_mouse->SetDataFormat(&c_dfDIMouse2);
	//	if (FAILED(hr))
	//		return hr;

	//	hr = m_mouse->SetCooperativeLevel(window_handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	//	if (FAILED(hr))
	//		return hr;

	//	//if (!bImmediate)
	//	{
	//		DIPROPDWORD dipdw;
	//		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	//		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	//		dipdw.diph.dwObj = 0;
	//		dipdw.diph.dwHow = DIPH_DEVICE;
	//		dipdw.dwData = 16; // Arbitrary buffer size

	//		if (FAILED(hr = m_mouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
	//			return hr;
	//	}

	//	m_keyboard->Acquire();
	//	m_mouse->Acquire();

	//	auto pos = RAIN_WINDOW->get_center_pos_absolute();
	//	x_center = u32(pos.x);
	//	y_center = u32(pos.y);

	//	return 0;
	//}

	//void InputWin32::shutdown()
	//{
	//	if (m_keyboard)
	//	{
	//		m_keyboard->Unacquire();
	//		m_keyboard->Release();
	//	}

	//	if (m_mouse)
	//	{
	//		m_mouse->Unacquire();
	//		m_mouse->Release();
	//	}

	//	if (m_diObject)
	//	{
	//		m_diObject->Release();
	//	}
	//}


	//void InputWin32::update()
	//{
	//	m_keyboard->Acquire();

	//	if (m_keysCurrent == &m_keysA)
	//	{
	//		m_keysCurrent = &m_keysB;
	//	}
	//	else
	//	{
	//		m_keysCurrent = &m_keysA;
	//	}

	//	ZeroMemory(m_keysCurrent->data(), m_keysCurrent->size());
	//	HRESULT hr = m_keyboard->GetDeviceState(m_keysCurrent->size(), m_keysCurrent->data());

	//	if (FAILED(hr))
	//	{
	//		hr = m_keyboard->Acquire();
	//		while (hr == DIERR_INPUTLOST)
	//		{
	//			hr = m_keyboard->Acquire();
	//		}
	//		if (FAILED(hr))
	//			return;

	//		m_keyboard->GetDeviceState(m_keysCurrent->size(), m_keysCurrent->data());
	//	}

	//	// MOUSE
	//	if (is_key_released(DIK_C))
	//	{
	//		mouse_lock = !mouse_lock;
	//	}

	//	DIMOUSESTATE2 dims2;
	//	ZeroMemory(&dims2, sizeof(dims2));

	//	hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE2),
	//		&dims2);
	//	if (FAILED(hr))
	//	{
	//		hr = m_mouse->Acquire();
	//		while (hr == DIERR_INPUTLOST)
	//		{
	//			hr = m_mouse->Acquire();
	//		}

	//		if (FAILED(hr))
	//			return;
	//	}

	//	x_offset = dims2.lX;
	//	y_offset = -dims2.lY;
	//	x_screen_pos += x_offset;
	//	y_screen_pos += y_offset;

	//	if (mouse_lock)
	//	{
	//		set_cursor_pos(x_center, y_center);
	//	}
	//}

	//void InputWin32::set_hdwn(const HWND _hdwn)
	//{
	//	window_handle = _hdwn;
	//}

	//void InputWin32::set_cursor_pos(u32 _x, u32 _y)
	//{
	//	::SetCursorPos(_x, _y);
	//}

}