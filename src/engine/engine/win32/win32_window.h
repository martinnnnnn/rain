#pragma once


#include <algorithm>

#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/win32/win32_helpers.h"


constexpr char* RAIN_WNDCLASSNAME = "RainClass";
constexpr char* RAIN_WNDNAME = "Rain Engine";


namespace rain::engine
{
	class Window
	{
	public:

        struct rect
        {
            i32 left;
            i32 top;
            i32 right;
            i32 bottom;
        };

		int init(HINSTANCE _hinstance, int _width, int _height, int _depth);
		void present();
		void shutdown();
		void show();
		void hide();
		void resize(int _width, int _height);
        void toggle_fullscreen();
        void fullscreen(bool _fullscreen);
		void recover_display_mode();
        rect get_rect();
		math::vec2 get_center_pos_absolute();

        math::vec2 size() const;

		bool initialized;
		HWND hwnd;

	private:

		i32 center_pos_x;
		i32 center_pos_y;
		HDC m_hdc;
		HGLRC m_hglrc;
		rect m_rect;
		bool full_screen;
		i32 m_width;
		i32 m_height;
		i32 m_depth;
	};

	LRESULT CALLBACK OGLWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
}
