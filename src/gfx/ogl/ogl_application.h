#pragma once

#include <Windows.h>

namespace rain
{
    struct OGLWindow;

    struct OGLApplication
    {
        OGLWindow* window;
    };

    void OGLInitApplication(OGLApplication* _application, HINSTANCE _hinstance);
    void OGLRunApplication(OGLApplication* _application);
    void OGLUpdate(OGLApplication* _application);
    void OGLRender(OGLApplication* _application);
}

