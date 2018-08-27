#pragma once

#include <windows.h>

#include <gl/gl.h>
#include <GL/GLU.h>


#define WINDOWED		0					
#define FULLSCREEN		1

#define SCRWIDTH		640					
#define SCRHEIGHT		480					
#define SCRDEPTH		16					

#define WNDCLASSNAME	"GLClass"			
#define WNDNAME			"OpenGL base code"	

HDC hdc;
HWND hwnd;
HGLRC hglrc;

int screenw = SCRWIDTH;
int screenh = SCRHEIGHT;
int screenmode = 0;

bool quit = false;

int indexPixelFormat = 0;

namespace rain
{
	void RenderFrame(void);
	void SysRecoverDisplayMode(void);
	void SysShutdown(void);
	void SysSetDisplayMode(int width, int height, int depth);
	void CreateWnd(HINSTANCE &hinst, int width, int height, int depth, int type);
	void SetGLFormat(void);
	void Resize(int width, int height);
	void InitOpenGL(void);
	int StartOpenGLRenderer(HINSTANCE hinstance);
	LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
}

