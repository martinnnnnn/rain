#include "win32_window.h"

#include <map>

DWORD err;


void Window::present()
{
	::SwapBuffers(m_hdc);
}

int Window::initialize(HINSTANCE _hinstance, int _width, int _height, int _depth)
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

	// TODO (martin) : handle windowed / full screen

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


LRESULT CALLBACK OGLWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

	switch (msg)
	{
	case WM_DESTROY:
	{
		GETWINDOW.shutdown();
		break;
	}

	case WM_SIZE:
	{
		GETWINDOW.resize(LOWORD(lparam), HIWORD(lparam));
		break;
	}
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);

}