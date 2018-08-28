#include "opengl_sandbox.h"

#include <time.h>
#include <stdlib.h>



namespace rain
{
	void RenderFrame(void)
	{
        static bool once = false;
        if (!once)
        {
            once = true;
            srand(time(NULL));
        }
        glClearColor((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void SysRecoverDisplayMode(void)
	{
		ChangeDisplaySettings(NULL, 0);
	}


	void SysShutdown(void)
	{
		wglMakeCurrent(hdc, NULL);
		wglDeleteContext(hglrc);

		PostQuitMessage(0);

		if (screenmode == FULLSCREEN)
			SysRecoverDisplayMode();
	}

	void SysSetDisplayMode(int width, int height, int depth)
	{
		DEVMODE dmode;

		memset(&dmode, 0, sizeof(DEVMODE));
		dmode.dmSize = sizeof(DEVMODE);
		dmode.dmPelsWidth = width;
		dmode.dmPelsHeight = height;
		dmode.dmBitsPerPel = depth;
		dmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		// change resolution, if possible
		if (ChangeDisplaySettings(&dmode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			screenmode = 0;

			MessageBox(NULL, "Your system doesn't support this screen resolution", "Display Error", MB_OK);
			SysShutdown();
		}
	}

	void CreateWnd(HINSTANCE &hinst, int width, int height, int depth, int type)
	{
		int posx = (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2);
		int posy = (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2);

		long wndStyle = WS_OVERLAPPEDWINDOW;
		screenmode = WINDOWED;

		if (type == FULLSCREEN)
		{
			wndStyle = WS_POPUP;
			screenmode = FULLSCREEN;

			posx = 0;
			posy = 0;

			SysSetDisplayMode(screenw, screenh, SCRDEPTH);
		}

		hwnd = CreateWindowExW(NULL,
			WNDCLASSNAME,
			WNDNAME,
			wndStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			posx, posy,
			width, height,
			NULL,
			NULL,
			hinst,
			NULL);
	}

	void Resize(int width, int height)
	{
		if (height <= 0)
			height = 1;

		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();

		gluPerspective(45.0f, (float)width / (float)height, 0.2f, 255.0f);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
	}

	LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
        case WM_PAINT:
        {
            RenderFrame();

            SwapBuffers(hdc);
            break;
        }
		case WM_DESTROY:
		{
			SysShutdown();
			break;
		}

		case WM_SIZE:
		{
			Resize(LOWORD(lparam), HIWORD(lparam));

			screenw = LOWORD(lparam);
			screenh = HIWORD(lparam);
			break;
		}
		}

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	int StartOpenGLRenderer(HINSTANCE hinstance)
	{
		MSG msg;
		WNDCLASSEXW ex;

		ex.cbSize = sizeof(WNDCLASSEXW);
		ex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		ex.lpfnWndProc = rain::WinProc;
		ex.cbClsExtra = 0;
		ex.cbWndExtra = 0;
		ex.hInstance = hinstance;
		ex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		ex.hCursor = LoadCursor(NULL, IDC_ARROW);
		ex.hbrBackground = NULL;
		ex.lpszMenuName = NULL;
		ex.lpszClassName = WNDCLASSNAME;
		ex.hIconSm = NULL;

		if (!RegisterClassExW(&ex))
		{
			MessageBox(NULL, "Failed to register the window class", "Window Reg Error", MB_OK);
			return 1;
		}

		CreateWnd(hinstance, screenw, screenh, SCRDEPTH, WINDOWED);

        if ((hdc = GetDC(hwnd)) == NULL)
        {
            MessageBox(hwnd, "Failed to Get the Window Device Context", "Device Context Error", MB_OK);
            SysShutdown();
            return 1;
        }

        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            SCRDEPTH,
            0,0,0,0,0,0,0,0,0,0,0,0,0, // useles parameters
            16,
            0,0,PFD_MAIN_PLANE,0,0,0,0
        };

        // Choose the closest pixel format available
        if (!(indexPixelFormat = ChoosePixelFormat(hdc, &pfd)))
        {
            MessageBox(hwnd, "Failed to find pixel format", "Pixel Format Error", MB_OK);
            SysShutdown();
        }

        // Set the pixel format for the provided window DC
        if (!SetPixelFormat(hdc, indexPixelFormat, &pfd))
        {
            MessageBox(hwnd, "Failed to Set Pixel Format", "Pixel Format Error", MB_OK);
            SysShutdown();
        }

        if ((hglrc = wglCreateContext(hdc)) == NULL)
        {
            MessageBox(hwnd, "Failed to Create the OpenGL Rendering Context", "OpenGL Rendering Context Error", MB_OK);
            SysShutdown();
            return 1;
        }

        if ((wglMakeCurrent(hdc, hglrc)) == false)
        {
            MessageBox(hwnd, "Failed to Make OpenGL Rendering Context Current", "OpenGL Rendering Context Error", MB_OK);
            SysShutdown();
        }

        Resize(SCRWIDTH, SCRHEIGHT);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glShadeModel(GL_SMOOTH);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

		while (!quit)
		{
			if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					quit = true;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
            InvalidateRect(hwnd, NULL, TRUE);
			

			if (GetAsyncKeyState(VK_ESCAPE))
				SysShutdown();
		}
		return msg.lParam;
	}
}


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
{
	return rain::StartOpenGLRenderer(hinstance);
}
