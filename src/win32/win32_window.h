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
#include "gfx/ogl/ogl_renderer.h"

#include <glm/glm.hpp>

#define RAIN_WNDCLASSNAME "RainClass"
#define RAIN_WNDNAME "Rain Engine"


namespace rain
{
	class Window
	{
	public:

		int init(HINSTANCE _hinstance, int _width, int _height, int _depth);
		void present();
		void shutdown();
		void show();
		void hide();
		void resize(int _width, int _height);
		void fullscreen(bool _fullscreen);
		void recover_display_mode();
		bool is_initialized() { return initialized; }
		glm::vec2 get_center_pos_absolute();
		HWND m_hwnd;

	private:

		int center_pos_x;
		int center_pos_y;
		HDC m_hdc;
		HGLRC m_hglrc;
		RECT m_rect;
		bool m_fullscreen;
		int m_width;
		int m_height;
		int m_depth;
		bool initialized;
	};

	LRESULT CALLBACK OGLWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
}