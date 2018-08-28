#pragma once


namespace rain
{
    struct OGLWindow;

    struct OGLApplication
    {
        OGLWindow* window;
    };

    void InitApplication(OGLApplication* _application)
    {

    }
    void OGLRunAppication(OGLApplication* _application);
    void OGLUpdate(OGLApplication* _application);
    void OGLRender(OGLApplication* _application);

}
