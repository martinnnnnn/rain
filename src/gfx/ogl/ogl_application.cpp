#include "ogl_application.h"

#include "ogl_window.h"




int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
{
    rain::OGLApplication application;
    rain::OGLInitApplication(&application, hinstance);
    rain::OGLRunApplication(&application);
    return 0;
}


namespace rain
{

    void OGLInitApplication(OGLApplication* _application, HINSTANCE _hinstance)
    {
        _application->window = (OGLWindow*)calloc(1, sizeof(OGLWindow));
        OGL_WINDOW_DESC desc;
        desc.hinstance = _hinstance;
        desc.width = 800;
        desc.height = 600;
        desc.depth = 16;
        desc.fullscreen = false;
        desc.WndClassName = L"rain";
        desc.WndName = L"Rain Engine";
        
        OGLInitWindow(&desc, _application->window);

    }

    void OGLRunApplication(OGLApplication* _application)
    {
        MSG msg;
        OGLWindow* window = _application->window;
        bool quit = false;
        while (!quit)
        {
            if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                    quit = true;

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            InvalidateRect(window->hwnd, NULL, TRUE);


            if (GetAsyncKeyState(VK_ESCAPE))
                OGLShutdown(window);
        }
    }

    void OGLUpdate(OGLApplication* _application)
    {

    }

    void OGLRender(OGLApplication* _application)
    {

    }
}

