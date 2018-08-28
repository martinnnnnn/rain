#include "ogl_window.h"

#include <gl/gl.h>
#include <GL/GLU.h>

#include <map>

namespace rain
{
    using OGLWindowMap = std::map< HWND, OGLWindow* >;
    using OGLWindowNameMap = std::map< std::wstring, OGLWindow* >;

    static OGLWindowMap s_OGLWindows;
    static OGLWindowNameMap s_OGLWindowsByName;


    void InitWindow(OGL_WINDOW_DESC _winDesc, OGLWindow* _window)
    {
        OGLRegisterWndClass(_winDesc, _window);
        OGLCreateWindow(_window);
        OGLCreateRenderContext(_window);
        OGLInitOpenGL();
    }

    int OGLRegisterWndClass(OGL_WINDOW_DESC _winDesc, OGLWindow* _window)
    {
        _window->hinstance = _winDesc.hinstance;
        _window->width = _winDesc.width;
        _window->height = _winDesc.depth;
        _window->fullscreen = _winDesc.fullscreen;
        _window->WndClassName = (wchar_t*)malloc((sizeof(_winDesc.WndClassName) + 1));
        wcsncpy(_window->WndClassName, _winDesc.WndClassName, sizeof(_window->WndClassName) + 1);
        _window->WndName = (wchar_t*)malloc((sizeof(_winDesc.WndName) + 1));
        wcsncpy(_window->WndName, _winDesc.WndName, sizeof(_window->WndName) + 1);

        MSG msg;
        WNDCLASSEXW ex;

        ex.cbSize = sizeof(WNDCLASSEXW);
        ex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        ex.lpfnWndProc = WinProc;
        ex.cbClsExtra = 0;
        ex.cbWndExtra = 0;
        ex.hInstance = _window->hinstance;
        ex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        ex.hCursor = LoadCursor(NULL, IDC_ARROW);
        ex.hbrBackground = NULL;
        ex.lpszMenuName = NULL;
        ex.lpszClassName = _window->WndClassName;
        ex.hIconSm = NULL;

        if (!RegisterClassExW(&ex))
        {
            MessageBox(NULL, "Failed to register the window class", "Window Reg Error", MB_OK);
            return 1;
        }
        return 0;
    }

    int OGLCreateWindow(OGLWindow* _window)
    {
        int posx = (GetSystemMetrics(SM_CXSCREEN) / 2) - (_window->width / 2);
        int posy = (GetSystemMetrics(SM_CYSCREEN) / 2) - (_window->height / 2);

        long wndStyle = WS_OVERLAPPEDWINDOW;
        

        if (_window->fullscreen)
        {
            wndStyle = WS_POPUP;

            posx = 0;
            posy = 0;

            DEVMODE dmode;

            memset(&dmode, 0, sizeof(DEVMODE));
            dmode.dmSize = sizeof(DEVMODE);
            dmode.dmPelsWidth = _window->width;
            dmode.dmPelsHeight = _window->height;
            dmode.dmBitsPerPel = _window->depth;
            dmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

            // change resolution, if possible
            if (ChangeDisplaySettings(&dmode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                _window->fullscreen = false;

                MessageBox(NULL, "Your system doesn't support this screen resolution", "Display Error", MB_OK);
                OGLShutdown(_window);
            }
        }

        _window->hwnd = CreateWindowExW(NULL,
            _window->WndClassName,
            _window->WndName,
            wndStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
            posx, posy,
            _window->width, _window->height,
            NULL,
            NULL,
            _window->hinstance,
            NULL);

        if ((_window->hdc = GetDC(_window->hwnd)) == NULL)
        {
            MessageBox(_window->hwnd, "Failed to Get the Window Device Context", "Device Context Error", MB_OK);
            OGLShutdown(_window);
            return 1;
        }
        return 0;
    }

    int OGLCreateRenderContext(OGLWindow* _window)
    {
        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            _window->depth,
            0,0,0,0,0,0,0,0,0,0,0,0,0, // useles parameters
            16,
            0,0,PFD_MAIN_PLANE,0,0,0,0
        };

        int indexPixelFormat = 0;
        // Choose the closest pixel format available
        if (!(indexPixelFormat = ChoosePixelFormat(_window->hdc, &pfd)))
        {
            MessageBox(_window->hwnd, "Failed to find pixel format", "Pixel Format Error", MB_OK);
            OGLShutdown(_window);
        }

        // Set the pixel format for the provided window DC
        if (!SetPixelFormat(_window->hdc, indexPixelFormat, &pfd))
        {
            MessageBox(_window->hwnd, "Failed to Set Pixel Format", "Pixel Format Error", MB_OK);
            OGLShutdown(_window);
        }

        if ((_window->hglrc = wglCreateContext(_window->hdc)) == NULL)
        {
            MessageBox(_window->hwnd, "Failed to Create the OpenGL Rendering Context", "OpenGL Rendering Context Error", MB_OK);
            OGLShutdown(_window);
            return 1;
        }

        if ((wglMakeCurrent(_window->hdc, _window->hglrc)) == false)
        {
            MessageBox(_window->hwnd, "Failed to Make OpenGL Rendering Context Current", "OpenGL Rendering Context Error", MB_OK);
            OGLShutdown(_window);
        }
    }

    void OGLInitOpenGL()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glShadeModel(GL_SMOOTH);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    }

    void OGLShutdown(OGLWindow* _window)
    {
        wglMakeCurrent(_window->hdc, NULL);
        wglDeleteContext(_window->hglrc);

        PostQuitMessage(0);

        if (_window->fullscreen)
            ChangeDisplaySettings(NULL, 0);
    }

    void OGLResizeWindow(OGLWindow* _window, int _width, int _height)
    {
        if (_height <= 0)
            _height = 1;

        _window->width = _width;
        _window->height = _height;

        glViewport(0, 0, _width, _height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (float)_width / (float)_height, 0.2f, 255.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void OGLShowWindow(OGLWindow* _window)
    {
        ShowWindow(_window->hwnd, SW_SHOW);
        UpdateWindow(_window->hwnd);
    }

    void OGLRefreshWindow(OGLWindow* _window)
    {
        InvalidateRect(_window->hwnd, NULL, TRUE);
    }


    //    CreateWnd(hinstance, screenw, screenh, SCRDEPTH, WINDOWED);

    //    if ((hdc = GetDC(hwnd)) == NULL)
    //    {
    //        MessageBox(hwnd, "Failed to Get the Window Device Context", "Device Context Error", MB_OK);
    //        SysShutdown();
    //        return 1;
    //    }

    //    PIXELFORMATDESCRIPTOR pfd =
    //    {
    //        sizeof(PIXELFORMATDESCRIPTOR),
    //        1,
    //        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    //        PFD_TYPE_RGBA,
    //        SCRDEPTH,
    //        0,0,0,0,0,0,0,0,0,0,0,0,0, // useles parameters
    //        16,
    //        0,0,PFD_MAIN_PLANE,0,0,0,0
    //    };

    //    int indexPixelFormat = 0;
    //    // Choose the closest pixel format available
    //    if (!(indexPixelFormat = ChoosePixelFormat(hdc, &pfd)))
    //    {
    //        MessageBox(hwnd, "Failed to find pixel format", "Pixel Format Error", MB_OK);
    //        SysShutdown();
    //    }

    //    // Set the pixel format for the provided window DC
    //    if (!SetPixelFormat(hdc, indexPixelFormat, &pfd))
    //    {
    //        MessageBox(hwnd, "Failed to Set Pixel Format", "Pixel Format Error", MB_OK);
    //        SysShutdown();
    //    }

    //    if ((hglrc = wglCreateContext(hdc)) == NULL)
    //    {
    //        MessageBox(hwnd, "Failed to Create the OpenGL Rendering Context", "OpenGL Rendering Context Error", MB_OK);
    //        SysShutdown();
    //        return 1;
    //    }

    //    if ((wglMakeCurrent(hdc, hglrc)) == false)
    //    {
    //        MessageBox(hwnd, "Failed to Make OpenGL Rendering Context Current", "OpenGL Rendering Context Error", MB_OK);
    //        SysShutdown();
    //    }

    //    Resize(SCRWIDTH, SCRHEIGHT);

    //    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //    glClearDepth(1.0f);

    //    glEnable(GL_DEPTH_TEST);
    //    glDepthFunc(GL_LEQUAL);

    //    glShadeModel(GL_SMOOTH);

    //    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    //    ShowWindow(hwnd, SW_SHOW);
    //    UpdateWindow(hwnd);

    //    while (!quit)
    //    {
    //        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
    //        {
    //            if (msg.message == WM_QUIT)
    //                quit = true;

    //            TranslateMessage(&msg);
    //            DispatchMessage(&msg);
    //        }
    //        InvalidateRect(hwnd, NULL, TRUE);


    //        if (GetAsyncKeyState(VK_ESCAPE))
    //            SysShutdown();
    //    }
    //    return msg.lParam;
    //}
    //}


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
}