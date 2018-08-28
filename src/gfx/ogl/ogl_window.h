#pragma once

#include "core/types.h"
#include <Windows.h>

namespace rain
{
    struct OGL_WINDOW_DESC
    {
        HINSTANCE hinstance;
        u32 width;
        u32 height;
        u32 depth;
        bool fullscreen;
        wchar_t* WndClassName;
        wchar_t* WndName;
    };

    struct OGLWindow
    {
        HINSTANCE hinstance;
        HDC hdc;
        HWND hwnd;
        HGLRC hglrc;
        u32 width;
        u32 height;
        u32 depth;
        bool fullscreen;
        wchar_t* WndClassName;
        wchar_t* WndName;
    };

    LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void OGLInitWindow(OGL_WINDOW_DESC _winDesc, OGLWindow* _window);
    int OGLRegisterWndClass(OGL_WINDOW_DESC _winDesc, OGLWindow* _window);
    int OGLCreateWindow(OGLWindow* _window);
    int OGLCreateRenderContext(OGLWindow* _window);
    void OGLInitOpenGL();
    void OGLShutdown(OGLWindow* _window);
    void OGLResizeWindow(OGLWindow* _window, int _width, int _height);
    void OGLShowWindow(OGLWindow* _window);
    void OGLRefreshWindow(OGLWindow* _window);
}