#include "ogl_application.h"

#include "ogl_window.h"


namespace rain
{

    void InitApplication(OGLApplication* _application, HINSTANCE _hinstance)
    {
        _application->window = (OGLWindow*)malloc(sizeof(OGLWindow));
        OGLRegisterWndClass(_application->window, _hinstance);
        OGLCreateWindow(_application->window);
        OGLCreateRenderContext(_application->window);
    }

    void OGLRunAppication(OGLApplication* _application)
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
