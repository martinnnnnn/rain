#pragma once


#include <algorithm>
#include <glm/glm.hpp>

#include "core/singleton.h"
#include "gfx/ogl/ogl_renderer.h"
#include "win32/win32_helper.h"


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

        glm::u32vec2 size() const;

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

#define RAIN_WINDOW rain::Application::get().window
