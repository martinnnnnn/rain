#pragma once


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>

#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

#include <algorithm>

#include "core/singleton.h"
#include "ogl/ogl_renderer.h"

#define RAIN_WNDCLASSNAME "RainClass"
#define RAIN_WNDNAME "Rain Engine"


class Window : public Singleton<Window>
{
public:

    int initialize(HINSTANCE _hinstance, int _width, int _height, int _depth);
	void present();
	void shutdown();
	void show();
	void hide();
	void resize(int _width, int _height);
	void fullscreen(bool _fullscreen);
	void recover_display_mode();
    bool is_initialized() { return initialized; }


private:

    HWND m_hwnd;
    HDC m_hdc;
    HGLRC m_hglrc;
    bool m_fullscreen;
    RECT m_rect;
    int m_width;
    int m_height;
    int m_depth;
	bool initialized;
};

LRESULT CALLBACK OGLWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


#define GETWINDOW Window::Get()