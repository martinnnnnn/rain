#include "win32_window.h"

#include "win32/win32_input.h"
#include "win32/win32_application.h"

#include <strsafe.h>
#include <map>

namespace rain
{
    void Window::present()
    {
	    ::SwapBuffers(m_hdc);
    }

    int Window::init(HINSTANCE _hinstance, int _width, int _height, int _depth)
    {
	    m_hwnd = NULL;
	    m_hdc = NULL;
	    m_hglrc = NULL;
	    m_fullscreen = false;
	    m_rect = { 0 };
        initialized = false;

	    m_width = _width;
	    m_height = _height;
	    m_depth = _depth;

	    WNDCLASSEX ex;
	    ex.cbSize = sizeof(WNDCLASSEX);
	    ex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	    ex.lpfnWndProc = OGLWinProc;
	    ex.cbClsExtra = 0;
	    ex.cbWndExtra = 0;
	    ex.hInstance = _hinstance;
	    ex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	    ex.hCursor = LoadCursor(NULL, IDC_ARROW);
	    ex.hbrBackground = NULL;
	    ex.lpszMenuName = NULL;
	    ex.lpszClassName = RAIN_WNDCLASSNAME;
	    ex.hIconSm = NULL;

	    if (!RegisterClassEx(&ex))
	    {
		    MessageBox(NULL, "Failed to register the window class", "Window Reg Error", MB_OK);
		    return 1;
	    }

	    int posx = (GetSystemMetrics(SM_CXSCREEN) / 2) - (m_width / 2);
	    int posy = (GetSystemMetrics(SM_CYSCREEN) / 2) - (m_height / 2);

	    m_hwnd = CreateWindowEx(NULL,
		    RAIN_WNDCLASSNAME,
		    RAIN_WNDNAME,
		    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		    posx, posy,
		    m_width, m_height,
		    NULL,
		    NULL,
		    _hinstance,
		    NULL);

        m_rect.top = posy;
        m_rect.bottom = posy + m_height;
        m_rect.left = posx;
        m_rect.right = posx + m_width;

        center_pos_x = m_width / 2;
        center_pos_y = m_height / 2;


        if ((m_hdc = GetDC(m_hwnd)) == NULL)
        {
            MessageBox(m_hwnd, "Failed to Get the Window Device Context", "Device Context Error", MB_OK);
            return -1;
        }

        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //Flags
            PFD_TYPE_RGBA,												// The kind of framebuffer. RGBA or palette.
            32,															// Colordepth of the framebuffer.
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,															// Number of bits for the depthbuffer
            8,															// Number of bits for the stencilbuffer
            0,															// Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        int indexPixelFormat = 0;
        if (!(indexPixelFormat = ChoosePixelFormat(GetDC(m_hwnd), &pfd)))
        {
            MessageBox(m_hwnd, "Failed to find pixel format", "Pixel Format Error", MB_OK);
            return -1;
        }

        if (!SetPixelFormat(GetDC(m_hwnd), indexPixelFormat, &pfd))
        {
            MessageBox(m_hwnd, "Failed to Set Pixel Format", "Pixel Format Error", MB_OK);
            return -1;
        }

        if ((m_hglrc = wglCreateContext(m_hdc)) == NULL)
        {
            MessageBox(m_hwnd, "Failed to Create the OpenGL Rendering Context", "OpenGL Rendering Context Error", MB_OK);
            return -1;
        }

        if ((wglMakeCurrent(m_hdc, m_hglrc)) == false)
        {
            MessageBox(m_hwnd, "Failed to Make OpenGL Rendering Context Current", "OpenGL Rendering Context Error", MB_OK);
            return -1;
        }

        resize(m_width, m_height);


        initialized = true;

	    return 0;
    }

    void Window::show()
    {
	    ::ShowWindow(m_hwnd, SW_SHOW);
    }

    void Window::hide()
    {
	    ::ShowWindow(m_hwnd, SW_HIDE);
    }

    void Window::resize(int _width, int _height)
    {
	    m_width = std::max(1, _width);
	    m_height = std::max(1, _height);

	    glViewport(0, 0, m_width, m_height);
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluPerspective(45.0f, (float)m_width / (float)m_height, 0.2f, 255.0f);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
    }

    void Window::fullscreen(bool _fullscreen)
    {
        char buffer[500];
        sprintf_s(buffer, 500, "fullscreen ? %d\n", _fullscreen);
        OutputDebugString(buffer);
	    if (_fullscreen != m_fullscreen)
	    {
		    m_fullscreen = _fullscreen;

		    if (m_fullscreen)
		    {
			    ::GetWindowRect(m_hwnd, &m_rect);

			    UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

			    ::SetWindowLongW(m_hwnd, GWL_STYLE, windowStyle);

			    HMONITOR hMonitor = ::MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTONEAREST);
			    MONITORINFOEX monitorInfo = {};
			    monitorInfo.cbSize = sizeof(MONITORINFOEX);
			    ::GetMonitorInfo(hMonitor, &monitorInfo);

			    ::SetWindowPos(m_hwnd, HWND_TOPMOST,
				    monitorInfo.rcMonitor.left,
				    monitorInfo.rcMonitor.top,
				    monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
				    monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
				    SWP_FRAMECHANGED | SWP_NOACTIVATE);

			    ::ShowWindow(m_hwnd, SW_MAXIMIZE);
		    }
		    else
		    {
			    ::SetWindowLong(m_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

			    ::SetWindowPos(m_hwnd, HWND_NOTOPMOST,
				    m_rect.left,
				    m_rect.top,
				    m_rect.right - m_rect.left,
				    m_rect.bottom - m_rect.top,
				    SWP_FRAMECHANGED | SWP_NOACTIVATE);

			    ::ShowWindow(m_hwnd, SW_NORMAL);
		    }
	    }
    }

    void Window::shutdown()
    {
	    wglMakeCurrent(m_hdc, NULL);
	    wglDeleteContext(m_hglrc);

	    PostQuitMessage(0);
    }

    void Window::recover_display_mode()
    {
	    ChangeDisplaySettings(NULL, 0);
    }

    glm::vec2 Window::get_center_pos_absolute()
    {
        return glm::vec2(m_rect.left + center_pos_x, m_rect.top + center_pos_y);
    }


    InputEvent::MouseButtonType DecodeMouseButton(UINT messageID)
    {
        InputEvent::MouseButtonType mouseButton = InputEvent::MouseButtonType::None;
        switch (messageID)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        {
            mouseButton = InputEvent::MouseButtonType::Left;
        }
        break;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        {
            mouseButton = InputEvent::MouseButtonType::Right;
        }
        break;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
        {
            mouseButton = InputEvent::MouseButtonType::Middel;
        }
        break;
        }

        return mouseButton;
    }


    LRESULT CALLBACK rain::OGLWinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {

	    switch (msg)
	    {
	    case WM_DESTROY:
	    {
		    RAIN_WINDOW->shutdown();
		    break;
	    }

	    case WM_SIZE:
	    {
		    RAIN_WINDOW->resize(LOWORD(lParam), HIWORD(lParam));
		    break;
	    }

     //   case WM_SYSKEYDOWN:
     //   case WM_KEYDOWN:
     //   {
     //       MSG charMsg;
     //       // Get the Unicode character (UTF-16)
     //       unsigned int c = 0;
     //       // For printable characters, the next message will be WM_CHAR.
     //       // This message contains the character code we need to send the KeyPressed event.
     //       // Inspired by the SDL 1.2 implementation.
     //       if (PeekMessage(&charMsg, hwnd, 0, 0, PM_NOREMOVE) && charMsg.message == WM_CHAR)
     //       {
     //           GetMessage(&charMsg, hwnd, 0, 0);
     //           c = static_cast<unsigned int>(charMsg.wParam);
     //       }
     //       bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
     //       bool control = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
     //       bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
     //       KeyCode::Key key = (KeyCode::Key)wParam;
     //       unsigned int scanCode = (lParam & 0x00FF0000) >> 16;
     //       GETINPUT.add_input_event(InputEvent::create_keyboard_event(key, c, InputEvent::State::Pressed, shift, control, alt));
     //   }
     //   break;
     //   case WM_SYSKEYUP:
     //   case WM_KEYUP:
     //   {
     //       bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
     //       bool control = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
     //       bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
     //       KeyCode::Key key = (KeyCode::Key)wParam;
     //       unsigned int c = 0;
     //       unsigned int scanCode = (lParam & 0x00FF0000) >> 16;

     //       // Determine which key was released by converting the key code and the scan code
     //       // to a printable character (if possible).
     //       // Inspired by the SDL 1.2 implementation.
     //       unsigned char keyboardState[256];
     //       GetKeyboardState(keyboardState);
     //       wchar_t translatedCharacters[4];
     //       if (int result = ToUnicodeEx(static_cast<UINT>(wParam), scanCode, keyboardState, translatedCharacters, 4, 0, NULL) > 0)
     //       {
     //           c = translatedCharacters[0];
     //       }
     //       GETINPUT.add_input_event(InputEvent::create_keyboard_event(key, c, InputEvent::State::Released, control, shift, alt));
     //   }
     //   break;
     //   // The default window procedure will play a system notification sound 
     //   // when pressing the Alt+Enter keyboard combination if this message is 
     //   // not handled.
     //   case WM_SYSCHAR:
     //       break;
     //   case WM_MOUSEMOVE:
     //   {
     //       bool lButton = (wParam & MK_LBUTTON) != 0;
     //       bool rButton = (wParam & MK_RBUTTON) != 0;
     //       bool mButton = (wParam & MK_MBUTTON) != 0;
     //       bool shift = (wParam & MK_SHIFT) != 0;
     //       bool control = (wParam & MK_CONTROL) != 0;

     //       int x = ((int)(short)LOWORD(lParam) - 12);
     //       int y = ((int)(short)HIWORD(lParam) + 1);

     //       int newx = GETWINDOW.get_center_pos_absolute().x;
     //       int newy = GETWINDOW.get_center_pos_absolute().y;

     //       //GETINPUT.add_input_event(InputEvent::create_mouse_motion_event(x, y, lButton, rButton, mButton, control, shift));
     //       //::SetCursorPos(newx, newy);

     //       //char buffer[256];
     //       //sprintf_s(buffer, "received : (%d, %d)\n", x, y);
     //       //OutputDebugStringA(buffer);
     //       //sprintf_s(buffer, "center : (%d, %d)\n", newx / 2, newy / 2);
     //       //OutputDebugStringA(buffer);
     //   }
     //   break;
     //   case WM_LBUTTONDOWN:
     //   case WM_RBUTTONDOWN:
     //   case WM_MBUTTONDOWN:
     //   {
     //       bool lButton = (wParam & MK_LBUTTON) != 0;
     //       bool rButton = (wParam & MK_RBUTTON) != 0;
     //       bool mButton = (wParam & MK_MBUTTON) != 0;
     //       bool shift = (wParam & MK_SHIFT) != 0;
     //       bool control = (wParam & MK_CONTROL) != 0;

     //       int x = ((int)(short)LOWORD(lParam));
     //       int y = ((int)(short)HIWORD(lParam));

     //       GETINPUT.add_input_event(InputEvent::create_mouse_button_event(DecodeMouseButton(msg), InputEvent::State::Pressed, x, y, lButton, rButton, mButton, control, shift));
     //   }
     //   break;
     //   case WM_LBUTTONUP:
     //   case WM_RBUTTONUP:
     //   case WM_MBUTTONUP:
     //   {
     //       bool lButton = (wParam & MK_LBUTTON) != 0;
     //       bool rButton = (wParam & MK_RBUTTON) != 0;
     //       bool mButton = (wParam & MK_MBUTTON) != 0;
     //       bool shift = (wParam & MK_SHIFT) != 0;
     //       bool control = (wParam & MK_CONTROL) != 0;

     //       int x = ((int)(short)LOWORD(lParam));
     //       int y = ((int)(short)HIWORD(lParam));

     //       GETINPUT.add_input_event(InputEvent::create_mouse_button_event(DecodeMouseButton(msg), InputEvent::State::Released, x, y, lButton, rButton, mButton, control, shift));
     //   }
     //   break;
     //   case WM_MOUSEWHEEL:
     //   {
     //       // The distance the mouse wheel is rotated.
     //       // A positive value indicates the wheel was rotated to the right.
     //       // A negative value indicates the wheel was rotated to the left.
     //       float zDelta = ((int)(short)HIWORD(wParam)) / (float)WHEEL_DELTA;
     //       short keyStates = (short)LOWORD(wParam);

     //       bool lButton = (keyStates & MK_LBUTTON) != 0;
     //       bool rButton = (keyStates & MK_RBUTTON) != 0;
     //       bool mButton = (keyStates & MK_MBUTTON) != 0;
     //       bool shift = (keyStates & MK_SHIFT) != 0;
     //       bool control = (keyStates & MK_CONTROL) != 0;

     //       int x = ((int)(short)LOWORD(lParam));
     //       int y = ((int)(short)HIWORD(lParam));

     //       // Convert the screen coordinates to client coordinates.
     //       POINT clientToScreenPoint;
     //       clientToScreenPoint.x = x;
     //       clientToScreenPoint.y = y;
     //       ScreenToClient(hwnd, &clientToScreenPoint);

     //       //MouseWheelEventArgs mouseWheelEventArgs(zDelta, lButton, mButton, rButton, control, shift, (int)clientToScreenPoint.x, (int)clientToScreenPoint.y);
     //   }
     //   break;
	    }
	    return DefWindowProc(hwnd, msg, wParam, lParam);

    }
}
